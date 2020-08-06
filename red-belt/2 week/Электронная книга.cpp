#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class ReadingManager {
public:
	ReadingManager() : rating(1001, 0), user_ids(100'001, 0), user_counts(100'001, 0) {
		num_users = 0;
	}

  void Read(int user_id, int page_count) {
	  if (user_ids[user_id] == 0) {
		  user_ids[user_id] = 1;
		  user_counts[user_id] = page_count;
		  rating[page_count]++;
		  num_users++;
		  return;
	  }
	  else {
		  rating[user_counts[user_id]]--;
		  rating[page_count]++;
		  user_counts[user_id] = page_count;
	  }
  }

  double Cheer(int user_id) const {
	  if (num_users == 0 || user_ids.at(user_id) == 0) {
		  return 0.0;
	  }
	  if (num_users == 1 && user_ids.at(user_id) != 0) {
		  return 1;
	  }
	  double result = 0;
	  for (int i=1; i< user_counts.at(user_id); ++i) {
		  result += rating[i];
	  }
	  result /= (num_users - 1);
	  return result;
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> rating;
  vector<int> user_ids;
  vector<int> user_counts;
  int num_users;
};

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}