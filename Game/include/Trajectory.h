#pragma once

class Trajectory
{
public:
	void ModelTrajectories();
private:
	void CalcK1();
	void CalcK2();
	void CalcK3();
	void CalcK4();
};