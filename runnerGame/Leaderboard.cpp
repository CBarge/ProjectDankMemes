
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Leaderboard.h"

Leaderboard::Leaderboard(){
	filename = "Scores.txt";
}
Leaderboard::~Leaderboard(){

}

bool Leaderboard::leaderboardExists(){ //checks if a file
	
	std::ifstream myfile(filename.c_str());
	if (myfile.is_open()) //if file is found = true
	{
		return true;
	}
	else
		return false;
}
void Leaderboard::initLeaderboard(){
	if (leaderboardExists()){
		std::string line = "";
		std::ifstream myfile(filename.c_str());
		if (myfile.is_open()){
			while (getline(myfile, line)){//for every line in file
				int i = 0;
				int j = 0;
				std::string name = "";
				std::string score = "";
				while (line[i] != '\t'){//skips over rank
					i++;
				}
				i++;
				while (line[i] != '\t'){//finds name and adds to name vector
					name += line[i];
					i++;
				}
				i++;
				while (line[i] != '\n'){//finds score
					score += line[i];
					i++;
					j++;
				}
				int intScore = atoi(score.c_str);

				if (j > 9){ //stops if there are more than 10 scores in the leaderboard text file and only take the ones on the first 10 lines
					break;
				}
				scores.push_back(intScore);
				names.push_back(name);
			}
		}
	}
}
int Leaderboard::getRank(int score){
	if (leaderboardExists()){
		if (scores.size() == 10 && score < scores.at(9))
			return 0;
		else if (score >= scores.at(0))
			return 1;
		else{
			for (int i = scores.size() - 1; i >= 0; i--){
				if (score <= scores.at(i)){
					return i + 2;
				}
			}
			}
		}
	}
	else 
		return 1;
}

bool Leaderboard::checkHighScore(int score){
	if (getRank != 0) return true;
	else return false;
}

void Leaderboard::addHighScore(int score, std::string name){
	//should check in maingame using checkHighScore if this method should be called 
	//adds new high score to vectors tracking top 10 names and scores
	
	std::vector<int>::iterator itS = scores.begin(); //itS is Iterator Score
	scores.insert(itS + getRank(score) - 1, score);
	
	if (scores.size() > 10) 
		scores.pop_back;
	
	std::vector<std::string>::iterator itN = names.begin();
	names.insert(itN + getRank(score) - 1, name);
	
	if (names.size() > 10) 
		names.pop_back;

	std::string line = "";
	std::ofstream myfile;
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < scores.size(); i++)
	{
		std::string rank = std::to_string(i+1);
		std::string scoreStr = std::to_string(scores.at(i));
		myfile << rank + '\t' + names.at(i) + '\t' + scoreStr + '\n';
	}
	
}

std::string Leaderboard::LeaderboardToString(){
	std::string leaderboardStr = ""; 
	std::string line = "";
	std::ifstream myfile(filename.c_str());
	if (myfile.is_open()){
		while (getline(myfile, line)){
			leaderboardStr += line;
		}
	}
	return leaderboardStr;
}