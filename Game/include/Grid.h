#pragma once

class Grid
{
public:
	Grid();
	~Grid();
	void CheckIfOccupied();
	void CalculateLaplacian();
private:
	float*** gridInternal;
	bool*** occupiedInternal;
};