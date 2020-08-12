#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <future>
#include <thread>
using namespace std;

class InvertedIndex {
public:
  void Add(string &&document);
  vector<pair<size_t, size_t>> Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  unordered_map<string, vector<pair<size_t, size_t>>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  ~SearchServer();
  //void UpdateDocumentBaseOneThread(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStreamOneThread(istream& query_input, ostream& search_results_output);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
  vector<future<void>> futures;
  mutex mx;
};
