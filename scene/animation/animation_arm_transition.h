#pragma once

#include "animation_blend_tree.h"

class AnimationNodeArmTransition : public AnimationNodeSync {
	GDCLASS(AnimationNodeArmTransition, AnimationNodeSync);

	static const int in_idx = 0;
	static const int armed_idx = 1;
	static const int aiming_idx = 2;
	static const int near_wall_ids = 3;

    const StringName armed = PNAME("armed");
    const StringName aiming = PNAME("aiming");
    const StringName near_wall = PNAME("near_wall");

	double xfade_time = 0.3;

    double armed_weight = 0.0;
    double aiming_weight = 0.0;
    double near_wall_weight = 0.0;

	void _blend_layer(int p_index, float is_starting, double p_weight, AnimationMixer::PlaybackInfo p_pi, bool p_test_only);

protected:
	static void _bind_methods();

public:
	virtual void get_parameter_list(List<PropertyInfo> *r_list) const override;
	virtual Variant get_parameter_default_value(const StringName &p_parameter) const override;

	virtual String get_caption() const override;
    virtual NodeTimeInfo _process(const AnimationMixer::PlaybackInfo p_playback_info, bool p_test_only = false) override;

	void set_xfade_time(double p_fade);
	double get_xfade_time() const;

    virtual bool has_filter() const override;
	AnimationNodeArmTransition();
};
