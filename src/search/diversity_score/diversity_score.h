#ifndef DIVERSITY_SCORE_DIVERSITY_SCORE_H
#define DIVERSITY_SCORE_DIVERSITY_SCORE_H

#include "../search_space.h"
#include "../plan_manager.h"
#include "operator_interaction.h"

#include <unordered_set>
#include <unordered_map>
#include <list>

namespace options {
class OptionParser;
class Options;
}

enum class Aggregator {
	AVG,
	MIN
};

//namespace diversity_score {
typedef std::unordered_map<int, size_t> plan_set;
typedef std::unordered_map<int, char> subgoal_encode;

void add_diversity_score_options_to_parser(options::OptionParser &parser);
void add_diversity_score_subset_options_to_parser(options::OptionParser &parser);
void add_diversity_score_subset_bounded_options_to_parser(options::OptionParser &parser);
void add_diversity_score_subset_optimal_options_to_parser(options::OptionParser &parser);

class DiversityScore {
    const std::shared_ptr<AbstractTask> task;
    // Use task_proxy to access task information.
    TaskProxy task_proxy;

    StateRegistry state_registry;
    SearchSpace search_space;

    int cost_bound;
    bool plans_as_multisets;
    bool use_cache;
    bool similarity;
    bool reduce_labels;
    bool labels_lifted;
    bool reduce_skip_unmentioned;
    bool discounted_prefixes;
    float discount_factor;
    PlanManager plan_manager;

    std::unordered_map<size_t, std::unordered_map<size_t, float>> state_metric_cache;
    std::unordered_map<size_t, std::unordered_map<size_t, float>> stability_metric_cache;
    std::unordered_map<size_t, std::unordered_map<size_t, float>> uniqueness_metric_cache;
    std::unordered_map<size_t, std::unordered_map<size_t, float>> sgo_metric_cache;
    std::unordered_map<size_t, std::unordered_map<size_t, float>> flex_metric_cache;
    
    utils::HashMap<OperatorID, OperatorID> label_reduction;
    
    std::shared_ptr<OperatorInteraction> op_interaction;
    
    const Plan& get_plan(size_t ind) const;
    void get_operators_for_label(std::vector<OperatorID>& ids, std::string label, const std::unordered_map<std::string, OperatorID>& ops_by_names) const;
    bool is_label_matching(std::string label, std::string operator_name) const;
    size_t get_num_actions(const plan_set& set) const;
    size_t get_num_actions(size_t ind) const;
    void plan_to_set(plan_set &set_a, const Plan &plan, bool plans_as_multisets) const;

    float compute_stability_similarity_score(size_t plan_index1, size_t plan_index2);
    float compute_jaccard_similarity_score(const plan_set& set_a, const plan_set& set_b) const;

    float compute_state_similarity_score(size_t plan_index1, size_t plan_index2);
    float compute_state_similarity_score(const std::vector<StateID>& plan_a, const std::vector<StateID>& plan_b) const;
    float compute_state_similarity_score(StateID state1, StateID state2) const;

    float compute_uniqueness_similarity_score(size_t plan_index1, size_t plan_index2);
    float compute_uniqueness_similarity_score(const plan_set& set_a, const plan_set& set_b) const;

    float get_state_from_cache(size_t plan_index1, size_t plan_index2) const;
    void add_state_to_cache(size_t plan_index1, size_t plan_index2, float value);

    float get_stability_from_cache(size_t plan_index1, size_t plan_index2) const;
    void add_stability_to_cache(size_t plan_index1, size_t plan_index2, float value);

    float get_uniqueness_from_cache(size_t plan_index1, size_t plan_index2) const;
    void add_uniqueness_to_cache(size_t plan_index1, size_t plan_index2, float value);

    bool is_none_of_those(int var, int val) const;

    void prepare_plans();
    
    float compute_score_for_set_avg(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                    const std::vector<size_t>& selected_plan_indexes);
    
    float compute_score_for_set_min(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                    const std::vector<size_t>& selected_plan_indexes);
    
    float compute_discounted_prefix_similarity(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                               const Plan& plan1, const Plan& plan2, float gamma) const;
    
    std::vector<int> get_subgoal_sequence(std::vector<StateID> plan_trace);
    std::string encode_trace(std::vector<StateID> trace);
    float get_sgo_from_cache(size_t plan_index1, size_t plan_index2) const;
    void add_sgo_to_cache(size_t plan_index1, size_t plan_index2, float value);
    float compute_sgo_similarity_score(size_t plan_index1, size_t plan_index2);
    float compute_sgo_similarity_score(const Plan& plan_1, const Plan& plan_2);
    
    size_t check_interchangeability(const GlobalState& current, const Plan& plan, size_t from_index) const;
    std::vector<Plan> generate_partial_order_plans_from(const Plan& plan);
    std::set<std::set<int>> convert_plan_to_set_of_pops_sets(const std::vector<Plan>& plan);
    float get_flex_from_cache(size_t plan_index1, size_t plan_index2) const;
    void add_flex_to_cache(size_t plan_index1, size_t plan_index2, float value);
    float compute_flex_similarity_score(size_t plan_index1, size_t plan_index2);
    float compute_flex_similarity_score(const Plan& plan_1, const Plan& plan_2);
    
    // float compute_discounted_prefix_score_for_set_avg(bool stability, bool state, bool uniqueness,
    //     const vector<size_t>& selected_plan_indexes, float gamma) const;

    // float compute_discounted_prefix_score_for_set_min(bool stability, bool state, bool uniqueness,
    //     const vector<size_t>& selected_plan_indexes, float gamma) const;
    float compute_similarity_for_prefix_no_cache(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                                 const Plan& plan1, const Plan& plan2) const;
    
    
    void read_label_reduction(std::string file);

protected:
    int plans_seed_set_size;
    bool compute_states_metric;
    bool compute_stability_metric;
    bool compute_uniqueness_metric;
    bool compute_flex_metric;
    bool compute_sgo_metric;
    
    subgoal_encode subgoals_encoded_list;
    
    Aggregator aggregator_metric;
    bool dump_pairs;
    bool all_metrics;

    std::vector<std::vector<StateID>> plan_traces;
    std::vector<plan_set> plans_sets;
    std::vector<std::pair<size_t, int>> ordered_plan_indexes;
    std::vector<Plan> _plans;
    
    std::string get_metric_name(bool stability, bool state, bool uniqueness, bool sgo, bool flex) const;
    void print_plan(size_t ind);
    void print_plans(const std::vector<size_t>& selected_plan_indexes);
    void print_all_plans();
    void print_plans_json(const std::vector<size_t>& selected_plan_indexes, std::ostream& os);
    void print_all_plans_json(std::ostream& os);
    
    float compute_score_for_pair(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                 size_t plan_index1, size_t plan_index2);
    
    float compute_score_for_set(bool stability, bool state, bool uniqueness, bool sgo, bool flex,
                                const std::vector<size_t>& selected_plan_indexes);
    void compute_metrics_exact_set();
    OperatorID get_reduced_label(OperatorID op) const;

public:
    DiversityScore(const options::Options &opts);
    virtual ~DiversityScore() = default;

    PlanManager &get_plan_manager() {return plan_manager;}

    static void add_options_to_parser(options::OptionParser &parser);

    void read_plans();

    virtual void compute_metrics() = 0;

};

//}

#endif
