#include "./task.hpp"

class Task1: public AA::Task {
    public:
        void solve() override {
            read_problem_data();
            formulate_oracle_question();
            ask_oracle();
            decipher_oracle_answer();
            write_answer();
        }
    
        void read_problem_data() override {
        }
    
        void formulate_oracle_question() override {
        }
    
        void decipher_oracle_answer() override {
        }
    
        void write_answer() override {
        }

        ~Task1() override = default;
    
    private:
};

int main(void) {
    return 0;
}