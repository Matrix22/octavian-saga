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
            std::cin >> N >> M >> K;

            for (int subset_idx = 1; subset_idx <= M; ++subset_idx) {
                int subset_size;
                std::cin >> subset_size;

                for (int element_idx = 0; element_idx < subset_size; ++element_idx) {
                    int element;
                    std::cin >> element;

                    universe[element].insert(subset_idx);
                }
            }
        }
    
        void formulate_oracle_question() override {
            std::ofstream sat_file("sat.cnf");
            
            if (!sat_file.is_open()) {
                std::cerr << "Error opening sat.cnf file\n";
                exit(1);
            }

            // Write header file
            sat_file << "p cnf " << M * K << " " << K + M + K * M * (M - 1) / 2 + N << "\n";

            // Write clauses for each subset to be chosen for cover
            for (int iter_k = 1; iter_k <= K; ++iter_k) {
                for (int iter_m = 1; iter_m <= M; ++iter_m) {
                    sat_file << compute_bool_var(K, iter_m, iter_k) << " ";
                }
                sat_file << "0\n";
            }

            // Write clauses so one subset is chosen at most once
            if (K > 1) {
                for (int iter_m = 1; iter_m <= M; ++iter_m) {
                    for (int iter_k = 1; iter_k <= K; ++iter_k) {
                        sat_file << -compute_bool_var(K, iter_m, iter_k) << " ";
                    }
                    sat_file << "0\n";
                }
            }

            // Write clauses so just one subset is chosen for a step in the cover
            for (int iter_k = 1; iter_k <= K; ++iter_k) {
                for (int iter_m1 = 1; iter_m1 < M; ++iter_m1) {
                    for (int iter_m2 = iter_m1 + 1; iter_m2 <= M; ++iter_m2) {
                        sat_file << -compute_bool_var(K, iter_m1, iter_k) << " " << -compute_bool_var(K, iter_m2, iter_k) << " 0\n";
                    }
                }
            }

            // Write clauses so each element is covered by at least one subset
            for (const auto& [element, subsets]: universe) {
                for (const auto& subset_idx: subsets) {
                    for (int iter_k = 1; iter_k <= K; ++iter_k) {
                        sat_file << compute_bool_var(K, subset_idx, iter_k) << " ";
                    }
                }
                sat_file << "0\n";
            }

            sat_file.close();
        }
    
        void decipher_oracle_answer() override {
            std::ifstream sat_file("sat.sol");

            if (!sat_file.is_open()) {
                std::cerr << "Error opening sat.sol file\n";
                exit(1);
            }

            sat_file >> satisfiable;

            // Check if the problem is satisfiable
            if (satisfiable == "True") {
                int nr_of_bool_vars = 0;
                sat_file >> nr_of_bool_vars;

                if (nr_of_bool_vars != M * K) {
                    std::cerr << "Error: wrong number of boolean variables\n";
                    sat_file.close();

                    exit(1);
                }

                // If the boolean value is true then the subset is chosen
                // and the order in the cover is not taken into account
                for (int iter_m = 1; iter_m <= M; ++iter_m) {
                    if (chosen_subsets.size() == (size_t)K) {
                        break;
                    }

                    for (int iter_k = 1; iter_k <= K; ++iter_k) {
                        int bool_var;
                        sat_file >> bool_var;

                        if (bool_var > 0) {
                            chosen_subsets.insert(iter_m);

                            if (chosen_subsets.size() == (size_t)K) {
                                break;
                            }
                        }
                    }
                }
            }

            sat_file.close();
        }
    
        void write_answer() override {
            std::cout << satisfiable;

            if (satisfiable == "True") {

                // If the number of chosen subsets is less than K
                // then we insert random subsets until we have K
                // because the chosen subsets already cover
                // the universe
                while (chosen_subsets.size() < (size_t)K) {
                    chosen_subsets.insert(M--);
                }

                std::cout << "\n" << K << "\n";

                for (const auto& subset_idx: chosen_subsets) {
                    std::cout << subset_idx << " ";
                }
            }
        }

        ~Task1() override = default;
    
    private:
        int N, M, K;
        std::unordered_map<int, std::unordered_set<int>> universe;
        std::unordered_set<int> chosen_subsets;
        std::string satisfiable;

        /**
         * @brief Function to map a boolean variable to a number
         * in a DIMACS format.
         * 
         * @param k the number of chosen subsets
         * @param i the index of the subset
         * @param j the indes of the subset in the cover
         * @return int 
         */
        int compute_bool_var(int k, int i, int j) {
            return j + k * (i - 1);
        }
};

int main(void) {
    Task1().solve();
    
    return 0;
}