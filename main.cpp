#include <iostream>
#include <stdio.h>
#include <cstring>
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
                    cout << "  label = " << row["tag"] <<
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
            cout << "trained on " << num_post << " examples" << endl;
            // in is de_bug;
            if (is_debug){
                cout << "vocabulary size = " << word_in_post.size() << endl << endl;
            }
            
            if (is_debug){
                // cout classes section;
                cout << "classes:" << endl;
                for (auto it : label_in_post){
                    cout << "  " << it.first << ", " << 
                            it.second << " examples, " <<
                            "log-prior = " << log_prior(it.first) <<
                            endl;
                }
                // cout classifier parameters section;
                cout << "classifier parameters:" << endl;
                for (auto it_tag : label_contain_word){
                    cout << "  " << it_tag.first.first << ":" << 
                    it_tag.first.second << 
                    ", count = " << it_tag.second << 
                    ", log-likelihood = " << 
                    log_likelihood(it_tag.first.first, it_tag.first.second) << endl;
                }
                cout << endl;
            }
        }

        // prediction classifier;
        void test_part(string in_file){
            // cout the test data section;
            cout << "test data:" << endl;

            int num_test = 0;
            int match_row_test = 0;
            csvstream in_csv(in_file);
            std::map<string, string> row_test;
            // contain the log-probability of each tag;
            std::map<string, double> tag_prob;
            
            while (in_csv >> row_test){
                // calculate the number of test case;
                num_test++;
                // in each content, the unique name;
                std::set<string> words = unique_words(row_test["content"]);
                // every test case;
                for (auto it_tag : label_in_post){
                    // calculate each tag's probability;
                    tag_prob[it_tag.first] = log_probability(it_tag.first, words);
                }
                // cout the most likely;
                std::pair<string, double> output = max_tag_prob(tag_prob);
                cout << "  correct = " << row_test["tag"] << ", predicted = " <<
                    output.first << ", log-probability score = " <<
                    output.second << endl;
                cout << "  content = " << row_test["content"] << endl << endl;
                // chech if the output match the expected;
                if (row_test["tag"] == output.first) match_row_test++;
            }
            // check effiency;
            cout << "performance: " << match_row_test << " / " << num_test <<
            " posts predicted correctly" << endl;
        }

        // find the tag with maximum log-probability;
        std::pair<string, double> max_tag_prob (std::map<string, double> tag_prob_in){
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
            return log(label_in_post[tag_in] / (double)(num_post));
        }

        double log_likelihood(string tag_in, string word_in){
            // if word_in does not exist in anywhere;
            if (word_in_post.find(word_in) == word_in_post.end()){
                return log(1.0 / (double)(num_post));
            }
            // if word_in does not exist in with label_in, but somewhere else;
            if (label_contain_word.find({tag_in, word_in}) == label_contain_word.end()){
                return log(word_in_post[word_in] / (double)(num_post));
            }
            return log(label_contain_word[{tag_in, word_in}] 
                        / (double)label_in_post[tag_in]);
        }
        
        // calculate the log probability score of give post;
        double log_probability(string tag_in, std::set<string> words_in){
            // log(C) + log(wi|C);
            double log_prob_tag = log_prior(tag_in);
            for (auto it_word : words_in){
                log_prob_tag += log_likelihood(tag_in, it_word);
            }
            return log_prob_tag;
        }


    private:
        int num_post;
        // traning data;
        std::map<string, int> word_in_post;
        std::map<string, int> label_in_post;
        std::map<std::pair<string, string>, int> label_contain_word;
};

void warning(){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
};


int main(int argc, const char *argv[]) {
    
    // set precision;
    cout.precision(3);

    // check the input variables;
    if (argc < 3 || argc > 4) {
        warning();
        return -1;
    }
    if (argc == 4 && strcmp(argv[3], "--debug") != 0) {
        warning();
        return -1;
    }

    // check if "--debug" is required;
    bool is_debug = false;
    if (argc == 4) is_debug = true;

    // check the file could be open or not;
    string filename = argv[1];
    ifstream inf(argv[1]);
    if (!inf.is_open()){
        cout << "Error opening file: " << filename << endl;
        return -1;
    }

    // read train and test files;
    string train_data = argv[1];
    string test_data = argv[2];
    
    // run the train data;
    Classifier train_section(train_data, is_debug);
    train_section.test_part(test_data);
    
    return 1;
};