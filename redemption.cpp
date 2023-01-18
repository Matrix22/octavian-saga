#include "./SagaUtility.hpp"

class RedemptionUtility: public SAGA::AbstractUtility {
    public:
        void solve() override {
            read_problem_data();
            greedy_aproximation_algo();
            write_answer();
        }
    
        void read_problem_data() override {
            int N_CARDS = 0;

            std::cin >> N_CARDS >> N >> M;

            std::unordered_set<std::string> cards;
            std::unordered_map<std::string, int> universe;
            std::string card;

            // Read cards that are already in the deck
            for (int card_idx = 0; card_idx < N_CARDS; ++card_idx) {
                std::getline(std::cin >> std::ws, card);
                cards.insert(card);
            }

            // Read all cards in order to collect
            for (int card_idx = 1; card_idx <= N; ++card_idx) {
                std::getline(std::cin >> std::ws, card);
                universe.insert({card, card_idx});
            }

            // Remove all cards that are already in the deck
            // in order to create the universe to cover
            for (const auto card : cards) {
                universe.erase(card);
            }

            // Remap the string to int in order to process
            // the strings as elements from the universe and
            // to optimize time for comparisions
            int card_idx = 1;
            for (auto& card : universe) {
                card.second = card_idx++;
            }

            N = universe.size();

            // Register all the subsets with their elements as ints
            for (int subset_idx = 1; subset_idx <= M; ++subset_idx) {
                int subset_size;
                std::cin >> subset_size;

                for (int element_idx = 0; element_idx < subset_size; ++element_idx) {
                    std::getline(std::cin >> std::ws, card);

                    const auto universe_card = universe.find(card);
                    if (universe_card != universe.end()) {
                        subsets[subset_idx].push_back(universe_card->second);
                    }
                }
            }
        }

        void greedy_aproximation_algo() {
            std::unordered_set<int> dummy_universe;

            int max_covering_elements = 0;
            int max_covering_subset_idx = 0;

            // Select the fisrt subset as the one with the most elements
            // in order to optimize the code in the next while block
            for (const auto subset : subsets) {
                if ((int)subset.second.size() > max_covering_elements) {
                    max_covering_elements = subset.second.size();
                    max_covering_subset_idx = subset.first;
                }
            }

            if (max_covering_subset_idx == 0) {
                return;
            }

            // Insert all the elements from the first subset
            // into the `covered universe`
            for (const auto subset_element : subsets[max_covering_subset_idx]) {
                dummy_universe.insert(subset_element);
            }

            chosen_subsets.push_back(max_covering_subset_idx);
            subsets.erase(max_covering_subset_idx);

            // Select the next subset as the one with the most elements
            // that are not in the `covered universe`
            int covering_elements = 0;
            while ((int)dummy_universe.size() < N) {
                max_covering_elements = 0;
                max_covering_subset_idx = 0;

                for (const auto subset : subsets) {
                    covering_elements = 0;

                    // Compute the maximum number of elements
                    // that will be covered by the subset
                    for (const auto subset_element : subset.second) {
                        if (dummy_universe.count(subset_element) == 0) {
                            ++covering_elements;
                        }
                    }

                    if (covering_elements > max_covering_elements) {
                        max_covering_elements = covering_elements;
                        max_covering_subset_idx = subset.first;
                    }
                }

                // If no subset can cover any element from the universe
                // then the problem is unsatisfiable
                if (max_covering_subset_idx == 0) {
                    K = 0;
                    break;
                }

                ++K;

                for (const auto subset_element : subsets[max_covering_subset_idx]) {
                    dummy_universe.insert(subset_element);
                }

                chosen_subsets.push_back(max_covering_subset_idx);
                subsets.erase(max_covering_subset_idx);
            }
        }
    
        void write_answer() override {
            if (K != 0) {
                std::cout << K << "\n";

                for (const auto subset_idx : chosen_subsets) {
                    std::cout << subset_idx << " ";
                }
            }
        }

        ~RedemptionUtility() override = default;
    
    private:
        int N, M, K = 0;
        std::unordered_map<int, std::vector<int>> subsets;
        std::vector<int> chosen_subsets;

        void formulate_oracle_question() override { /* Nothing fot this function */ }
    
        void decipher_oracle_answer() override { /* Nothing fot this function */ }
};

int main(void) {
    RedemptionUtility().solve();

    return 0;
}
