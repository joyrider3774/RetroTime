#pragma once

#include <string>

bool SubmitScore(std::string secretkey, unsigned int gameid, unsigned int score, std::string playername);
std::string GetSubmitScoreUrl(std::string secretkey, unsigned int gameid, unsigned int score, std::string playername);