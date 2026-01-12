#include "animation_arm_transition.h"

void AnimationNodeArmTransition::get_parameter_list(List<PropertyInfo> *r_list) const {
	AnimationNode::get_parameter_list(r_list);
	r_list->push_back(PropertyInfo(Variant::BOOL, armed));
    r_list->push_back(PropertyInfo(Variant::BOOL, aiming));
    r_list->push_back(PropertyInfo(Variant::BOOL, near_wall));
}

Variant AnimationNodeArmTransition::get_parameter_default_value(const StringName &p_parameter) const {
	Variant ret = AnimationNode::get_parameter_default_value(p_parameter);
	if (ret != Variant()) {
		return ret;
	}

	return false;
}

String AnimationNodeArmTransition::get_caption() const {
	return "ArmTransition";
}

AnimationNode::NodeTimeInfo AnimationNodeArmTransition::_process(const AnimationMixer::PlaybackInfo p_playback_info, bool p_test_only) {
    AnimationMixer::PlaybackInfo pi = p_playback_info;
    double step = pi.delta / xfade_time;

    bool cur_armed = get_parameter(armed);
    bool cur_aiming = get_parameter(aiming);
    bool cur_near_wall = get_parameter(near_wall);

    bool is_armed = cur_armed && !cur_aiming;
    bool is_aiming = cur_armed && cur_aiming && !cur_near_wall;
    bool is_near_wall = cur_armed && cur_aiming && cur_near_wall;

    bool is_starting_armed = is_armed && armed_weight == 0;
    bool is_starting_aiming = is_aiming && aiming_weight == 0;
    bool is_starting_near_wall = is_near_wall && near_wall_weight == 0;

    armed_weight = Math::move_toward(armed_weight, is_armed ? 1 : 0.0, step);
    aiming_weight = Math::move_toward(aiming_weight, is_aiming ? 1 : 0.0, step);
    near_wall_weight = Math::move_toward(near_wall_weight, is_near_wall ? 1 : 0.0, step);
    
    pi.weight = 1.0 - (armed_weight + aiming_weight + near_wall_weight);
    NodeTimeInfo nti0 = blend_input(in_idx, pi, FILTER_BLEND, sync, p_test_only);

    _blend_layer(armed_idx, is_starting_armed, armed_weight, pi, p_test_only);
    _blend_layer(aiming_idx, is_starting_aiming, aiming_weight, pi, p_test_only);
    _blend_layer(near_wall_ids, is_starting_near_wall, near_wall_weight, pi, p_test_only);

    return nti0;
}

void AnimationNodeArmTransition::_blend_layer(int p_index, float is_starting, double p_weight, AnimationMixer::PlaybackInfo p_pi, bool p_test_only) {
    if (is_starting) {
        p_pi.weight = p_weight;
        p_pi.seeked = true;
        blend_input(p_index, p_pi, FILTER_PASS, true, p_test_only);
    } else if (!Math::is_zero_approx(p_weight)) {
        p_pi.weight = p_weight;
        p_pi.seeked = false;
        blend_input(p_index, p_pi, FILTER_PASS, sync, p_test_only);
    }
}

bool AnimationNodeArmTransition::has_filter() const {
	return true;
}

AnimationNodeArmTransition::AnimationNodeArmTransition() {
	add_input("in");
	add_input("armed");
    add_input("aiming");
    add_input("near_wall");
}

void AnimationNodeArmTransition::set_xfade_time(double p_fade) {
	xfade_time = p_fade;
}

double AnimationNodeArmTransition::get_xfade_time() const {
	return xfade_time;
}

void AnimationNodeArmTransition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_xfade_time", "time"), &AnimationNodeTransition::set_xfade_time);
	ClassDB::bind_method(D_METHOD("get_xfade_time"), &AnimationNodeTransition::get_xfade_time);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "xfade_time", PROPERTY_HINT_RANGE, "0,120,0.01,suffix:s"), "set_xfade_time", "get_xfade_time");
}
