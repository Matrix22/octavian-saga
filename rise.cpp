#include "./SagaUtility.hpp"

class RiseUtility: public SAGA::AbstractUtility {
    public:
        void solve() override {
            read_problem_data();
            formulate_oracle_question();
            ask_oracle();
            decipher_oracle_answer();
            write_answer();
        }
    
        void read_problem_data() override {
            int N_CARDS = 0;

            std::cin >> N_CARDS >> N >> M;

            std::unordered_set<std::string> cards;
            std::unordered_set<std::string> universe;
            std::string card;

            for (int card_idx = 0; card_idx < N_CARDS; ++card_idx) {
                std::getline(std::cin >> std::ws, card);
                cards.insert(card);
            }

            for (int card_idx = 0; card_idx < N; ++card_idx) {
                std::getline(std::cin >> std::ws, card);
                universe.insert(card);
            }

            for (const auto card : cards) {
                universe.erase(card);
            }

            N = universe.size();
            K = M;

            for (int subset_idx = 1; subset_idx <= M; ++subset_idx) {
                int subset_size;
                std::cin >> subset_size;

                for (int element_idx = 0; element_idx < subset_size; ++element_idx) {
                    std::getline(std::cin >> std::ws, card);

                    if (universe.find(card) != universe.end()) {
                        subsets[card].insert(subset_idx);
                    }
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
                    sat_file << COMPUTE_BOOL_VAR(K, iter_m, iter_k) << " ";
                }
                sat_file << "0\n";
            }

            // Write clauses so one subset is chosen at most once
            if (K > 1) {
                for (int iter_m = 1; iter_m <= M; ++iter_m) {
                    for (int iter_k = 1; iter_k <= K; ++iter_k) {
                        sat_file << -COMPUTE_BOOL_VAR(K, iter_m, iter_k) << " ";
                    }
                    sat_file << "0\n";
                }
            }

            // Write clauses so just one subset is chosen for a step in the cover
            for (int iter_k = 1; iter_k <= K; ++iter_k) {
                for (int iter_m1 = 1; iter_m1 < M; ++iter_m1) {
                    for (int iter_m2 = iter_m1 + 1; iter_m2 <= M; ++iter_m2) {
                        sat_file << -COMPUTE_BOOL_VAR(K, iter_m1, iter_k) << " " << -COMPUTE_BOOL_VAR(K, iter_m2, iter_k) << " 0\n";
                    }
                }
            }

            // Write clauses so each element is covered by at least one subset
            for (const auto& [element, subsets]: subsets) {
                for (const auto& subset_idx: subsets) {
                    for (int iter_k = 1; iter_k <= K; ++iter_k) {
                        sat_file << COMPUTE_BOOL_VAR(K, subset_idx, iter_k) << " ";
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

            std::string dummy_line;
            std::getline(sat_file, dummy_line);
            std::getline(sat_file, dummy_line);

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

            sat_file.close();
        }
    
        void write_answer() override {
            std::cout << chosen_subsets.size() << "\n";

            for (const auto subset_idx: chosen_subsets) {
                std::cout << subset_idx << " ";
            }
        }

        ~RiseUtility() override = default;
    
    private:
        int N, M, K;
        std::unordered_map<std::string, std::unordered_set<int>> subsets;
        std::unordered_set<int> chosen_subsets;
};

int main(void) {
    RiseUtility().solve();
    
    return 0;
}
