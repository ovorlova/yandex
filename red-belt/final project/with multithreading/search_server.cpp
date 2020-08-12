#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <future>
#include <thread>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return { make_move_iterator(istream_iterator<string>(words_input)), 
			 make_move_iterator(istream_iterator<string>()) };
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

SearchServer::~SearchServer() {
  for (auto& thread_ : futures) {
    thread_.get();
  }
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document); )
    new_index.Add(move(current_document));
  {
    std::lock_guard<std::mutex> lock(mx);
    index = move(new_index);
  }
}


void SearchServer::AddQueriesStreamOneThread(
    istream& query_input, ostream& search_results_output
) {
	  vector<size_t> docids(50000);
	  vector<size_t> inds(50000);
	  
	  for (string current_query; getline(query_input, current_query); ) {
		size_t ind = 0;
		for (const auto& word : SplitIntoWords(current_query)) {
		vector<pair<size_t, size_t>> vec;
		  {
          lock_guard<mutex> lock(mx);
          vec = index.Lookup(word);
          }
		  for (const auto& [docid, count] : vec)
		  	if (docids[docid] == 0) {
			  inds[ind++] = docid;
			}
			docids[docid] += count;
		  }
		}
		std::vector<std::pair<size_t, size_t>> search_results;
		for (size_t docid = 0; docid < ind; ++docid) {
		  search_results.push_back({inds[docid], docids[inds[docid]]});
		  inds[docid]=docids[inds[docid]]=0;
		}

        partial_sort(
            begin(search_results),
            begin(search_results) + min(static_cast<size_t>(5), search_results.size()),
            end(search_results),
            [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                int64_t lhs_docid = lhs.first;
                auto lhs_hit_count = lhs.second;
                int64_t rhs_docid = rhs.first;
                auto rhs_hit_count = rhs.second;
                return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
            }
        );

        search_results_output << current_query << ':';
        for (auto [docid, hitcount] : Head(search_results, 5)) {
			if (hitcount > 0) {
            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << hitcount << '}';
			}
        }
        search_results_output << endl;
    }
}

void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output
	) {
		  futures.push_back(std::async(launch::async,&SearchServer::AddQueriesStreamOneThread, this, 
		  ref(query_input), ref(search_results_output)));
	}

void InvertedIndex::Add(string &&document) {
  docs.push_back(move(document));
  
  for (const auto& word : SplitIntoWords(docs.back())) {
    auto& it = index[word];
    if (it.empty() ||  it.back().first != docs.size() - 1 ){
      it.push_back({docs.size() - 1, 1});
    }
	else{
		++it.back().second;
	}
  }
}

vector<pair<size_t, size_t>> InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    else {
        return {};
    }
}
