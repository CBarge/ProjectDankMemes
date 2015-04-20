#pragma once
#include <string>
#include <vector>

class Leaderboard{
public:
	Leaderboard();
	~Leaderboard();

	void initLeaderboard(); //at game startup, defines vectors scores and names if file exists
	bool leaderboardExists();
	int getRank(int score);
	bool checkHighScore(int score);
	void addHighScore(int score, std::string name);
	std::string LeaderboardToString();

private:
	std::string filename;
	int* scores;
	std::string* names;
};