#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <cmath>
#include "csvstream.hpp"

using namespace std;

set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}

class Classifier{
    public:
        // tranning classifier;
        Classifier(string in_file, bool is_debug){
            num_post = 0;
            csvstream in_csv(in_file);
            std::map<string, string> row;

            // check does it requries [debug];
            if (is_debug){
                cout << "training data:" << endl;
            }
            while (in_csv >> row){
                // show every post;
                if (is_debug){
                    cout << "label = " << row["tag"] <<
                ", content = " << row["content"] << endl;
                }
                // calculate how many post;
                num_post++;
                std::set<string> words = unique_words(row["content"]);
                // calculate how many post have the tag name;
                ++label_in_post[row["tag"]];
                for (auto word : words){
                    // calculate the number of how many post have the word;
                    ++word_in_post[word];
                    // calculate the number of posts contain the tag and the word;
                    ++label_contain_word[{row["tag"], word}];
                }
            }
            cout << "trained on " << num_post << " examples";
            // in is de_bug;
            if (is_debug){
                cout << "vocabulary size = " << word_in_post.size() << endl << endl;
            }
            
            if (is_debug){
                // cout classes section;
                cout << "classes:" << endl;
                for (auto it : label_in_post){
                    cout << " " << it.first << ", " << 
                            it.second << " examples, " <<
                            "log-prior = " << log_prior(it.first) <<
                            endl;
                }
                // cout classifier parameters section;
                cout << "classifier parameters:" << endl;
                for (auto it_tag : label_in_post){
                    for (auto it_word : word_in_post){
                        cout << it_tag.first << ":" << it_word.first << 
                        ", count = " << it_word.second << 
                        ", log-likelihood = " << 
                        log_likelihood(it_tag.first, it_word.first) << endl;
                    }
                    
                }
                cout << endl;
            }
        }

        // prediction classifier;
        Classifier(string in_file){
            // cout the test data section;
            cout << "test data:" << endl;
            int num_test = 0;
            csvstream in_csv(in_file);
            std::map<string, string> row_test;
            // contain the log-probability of each tag;
            std::map<string, double> tag_prob;
            // log-probability of each label;
            double temp_prob;

            while (in_csv >> row_test){
                // calculate how many tests are inserted;
                num_test++;
                std::set<string> words = unique_words(row_test["content"]);
                // calculate how many post have the tag name;
                ++label_in_post_test[row_test["tag"]];
                for (auto word : words){
                    // calculate the number of how many post have the word;
                    ++word_in_post_test[word];
                    // calculate the number of posts contain the tag and the word;
                    ++label_contain_word_test[{row_test["tag"], word}];
                }
                // contain unique words in each content;
                std::set<string> words = unique_words(row_test["content"]);
                tag_prob[row_test["tag"]] = log_probability(row_test["tag"]);
                // to be continue;
            }
        }

        // find the tag with maximum log-probability;
        std::pair<string, double> max_log_prob (std::map<string, double> tag_prob_in){
            std::string max_tag = (*tag_prob_in.begin()).first;
            double max_prob = (*tag_prob_in.begin()).second;
            for (auto it : tag_prob_in){
                if (it.second > max_prob){
                    max_tag = it.first;
                    max_prob = it.second;
                }
            }
            return {max_tag, max_prob};
        }

        double log_prior(string tag_in){
            return log(label_in_post[tag_in]/static_cast<double>(num_post));
        }

        double log_likelihood(string tag_in, string word_in){
            // if word_in does not exist in anywhere;
            if (word_in_post.find(word_in) == word_in_post.end()){
                return log(1/static_cast<double>(num_post));
            }
            // if word_in does not exist in with label_in, but somewhere else;
            if (label_contain_word.find({tag_in, word_in}) == label_contain_word.end()){
                return log(word_in_post[word_in]/static_cast<double>(num_post));
            }
            return log(label_contain_word[{tag_in, word_in}]/label_in_post[tag_in]);
        }
        
        // calculate the log probability score of give post;
        double log_probability(string tag_in){
            // log(C) + log(wi|C);
            double log_prob_tag = log_prior(tag_in);
            for (auto it_word : word_in_post){
                log_prob_tag += log_likelihood(tag_in, it_word.first);
            }
            return log_prob_tag;
        }


    private:
        int num_post;
        // traning data;
        std::map<string, int> word_in_post;
        std::map<string, int> label_in_post;
        std::map<std::pair<string, string>, int> label_contain_word;

        // testing data;
        std::map<string, int> word_in_post_test;
        std::map<string, int> label_in_post_test;
        std::map<std::pair<string, string>, int> label_contain_word_test;
};

void warning(){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
};


int main(int argc, char **argv) {
    
    // set precision;
    cout.precision(3);

    // check the input variables;
    if (argc != 3 || argc !=4) {
        warning();
        return -1;
    }
    if (argc == 4 && !strcmp(argv[3], "--debug")) {
        warning();
        return -1;
    }
    // check the file could be open or not;
    string filename = argv[1];
    ifstream inf(argv[1]);
    if (!inf.is_open()){
        cout << "Error opening file: " << filename << endl;
        return -1;
    }
};