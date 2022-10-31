#pragma once


namespace Yasuo {
	/*static bool AutoCast = false;
	static bool SkillDamage = false;
	static double Last_E_Pressed = 0;*/
	static double Q_Pressed = 0;
	static double Q_Pressed_Frames = -1;
	static bool Q_Aim = false;
	static bool Q_Lasthit = false;

	static bool UseQinEWhenFlash = false;

	static bool SkillDamage = false;

	void Initialize();
	void MainThread();
	void OnMenu();
	void OnDraw();
}
