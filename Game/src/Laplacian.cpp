#include "Laplacian.h"
#include <iostream>
#include <fstream>

#include <shlobj.h>
#include <windows.h>
#include <string>
#include <algorithm>
#pragma comment(lib, "shell32.lib")

Laplacian::Laplacian(int iterations, int gridX, int gridY, int gridZ, float gridScaling) : w(0.94f), threshold_(1e-6), path_()
{
	iterations_ = iterations;
	gridX_ = gridX;
	gridY_ = gridY;
	gridZ_ = gridZ;
	gridScaling_ = gridScaling;

	grid = new float** [gridX];
	occupiedInternal = new bool** [gridX];
	electricField = new float*** [gridX];

	for (size_t i = 0; i < gridX; i++)
	{
		grid[i] = new float*[gridY];

		for (size_t j = 0; j < gridY; j++)
		{
			grid[i][j] = new float[gridZ];
		}
	}

	for (size_t i = 0; i < gridX; i++)
	{
		for (size_t j = 0; j < gridY; j++)
		{
			for (size_t k = 0; k < gridZ; k++)
			{
				grid[i][j][k] = 0;
			}
		}
	}

	for (size_t i = 0; i < gridX; i++)
	{
		occupiedInternal[i] = new bool* [gridY];

		for (size_t j = 0; j < gridY; j++)
		{
			occupiedInternal[i][j] = new bool[gridZ];
		}
	}

	for (size_t i = 0; i < gridX; i++)
	{
		for (size_t j = 0; j < gridY; j++)
		{
			for (size_t k = 0; k < gridZ; k++)
			{
				occupiedInternal[i][j][k] = false;
			}
		}
	}
	//E-Field
	for (size_t i = 0; i < gridX; i++)
	{
		electricField[i] = new float** [gridY];

		for (size_t j = 0; j < gridY; j++)
		{
			electricField[i][j] = new float*[gridZ];
			for (size_t k = 0; k < gridZ_; k++)
			{
				electricField[i][j][k] = new float[3];
			}
		}
	}

	for (size_t i = 0; i < gridX; i++)
	{
		for (size_t j = 0; j < gridY; j++)
		{
			for (size_t k = 0; k < gridZ; k++)
			{
				for (size_t w = 0; w < 3; w++)
				{
					electricField[i][j][k][w] = 0;
				}
			}
		}
	}
}

Laplacian::~Laplacian()
{
	for (size_t i = 0; i < gridX_; i++)
	{
		for (size_t j = 0; j < gridY_; j++)
		{
			for (size_t k = 0; k < gridZ_; k++)
			{
				delete[] electricField[i][j][k];
			}

			delete[] grid[i][j];
			delete[] occupiedInternal[i][j];
			delete[] electricField[i][j];
		}
		delete[] grid[i];
		delete[] occupiedInternal[i];
		delete[] electricField[i];
	}
	delete[] grid;
	delete[] occupiedInternal;
	delete[] electricField;
}

void Laplacian::Calculate(std::vector<std::unique_ptr<Shape>>& objectList)
{
	std::cout << "Calculating potential" << '\n';
	CheckOccupation(objectList);

	for (size_t b = 0; b < iterations_; b++)
	{
		for (size_t i = 1; i < gridX_-1; i++)
		{
			for (size_t j = 1; j < gridY_-1; j++)
			{
				for (size_t k = 1; k < gridZ_-1; k++)
				{
					if (!occupiedInternal[i][j][k])
					{
						grid[i][j][k] = (1.0f + w) * (grid[i + 1][j][k] + grid[i - 1][j][k] + grid[i][j + 1][k] + grid[i][j - 1][k] +
							grid[i][j][k + 1] + grid[i][j][k - 1]) / 6.0f - w * grid[i][j][k];
					}
				}
			}
		}
		std::cout << (float)(b+1) * 100.0f / (float)iterations_ << '%' << '\n';
	}

	std::cout << "Calculating Electric Field" << '\n';

	for (size_t i = 2; i < gridX_ - 2; i++)
	{
		for (size_t j = 2; j < gridY_ - 2; j++)
		{
			for (size_t k = 2; k < gridZ_ - 2; k++)
			{
				// 0 = x, 1 = y, 2 = z

				electricField[i][j][k][0] = -1*(grid[i + 1][j][k] - grid[i - 1][j][k]) * gridScaling_ / 2;
				electricField[i][j][k][1] = -1*(grid[i][j + 1][k] - grid[i][j - 1][k]) * gridScaling_ / 2;
				electricField[i][j][k][2] = -1*(grid[i][j][k + 1] - grid[i][j][k - 1]) * gridScaling_ / 2;
			}
		}
	}
}

void Laplacian::CheckOccupation(std::vector<std::unique_ptr<Shape>>& objectList)
{
	for (size_t i = 1; i < gridX_ - 1; i++)
	{
		for (size_t j = 1; j < gridY_ - 1; j++)
		{
			for (size_t k = 1; k < gridZ_ - 1; k++)
			{
				for (size_t obj = 0; obj < objectList.size(); obj++)
				{
					if ((*objectList[obj]).CheckPointForOccupation(i, j, k, gridScaling_))
					{
						occupiedInternal[i][j][k] = true;
						grid[i][j][k] = (*objectList[obj]).voltage;
					}
				}
			}
		}
	}
}

void Laplacian::DebugPrint()
{
	for (size_t i = 0; i < 15; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			for (size_t k = 0; k < 15; k++)
			{
				std::cout << "i: " << i << '\n' << "j: " << j << '\n' << "k: " << k << '\n' << "Value: " << grid[i][j][k] << '\n';
			}
		}
	}
}

void Laplacian::ExportCSV()
{
	std::ofstream output;

	wchar_t my_documents[1024];
	HRESULT result = SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (SUCCEEDED(result))
	{
		char str[1024];
		wcstombs(str, my_documents, 1023);

		path_ = str;
		eFieldPath_ = str;
		std::replace(path_.begin(), path_.end(), '\\', '/');
		std::replace(eFieldPath_.begin(), eFieldPath_.end(), '\\', '/');
		path_.append("/potential.csv");
		eFieldPath_.append("/efield.csv");
	}
	else
	{
		std::cout << "Error grabbing documents folder path" << '\n';
		return;
	}

	output.open(path_);

	//for (size_t i = 1; i < gridX_-1; i++)
	//{
	//	for (size_t j = 1; j < gridY_-1; j++)
	//	{
	//		//output << i << "," << j << "," << grid[i][j][(int)(gridZ_ / 2)] << '\n';
	//	}
	//}

	for (size_t j = 1; j < gridY_ - 1; j++)
	{
		for (size_t i = 1; i < gridX_ - 2; i++)
		{
			//output << i << "," << j << "," << grid[i][j][(int)(gridZ_ / 2)] << '\n';

			//if (std::abs(grid[i][j][(int)(gridZ_ / 2)]) < 0.00001)
			//{
			//	output << 0 << ',';
			//}
			//else
			//{
			//	output << grid[i][j][(int)(gridZ_ / 2)] << ',';
			//}

			output << grid[i][j][(int)(gridZ_ / 2)] << ',';

		}
		//if (std::abs(grid[gridX_ - 1][j][(int)(gridZ_ / 2)]) < 0.00001)
		//{
		//	output << 0 << '\n';
		//}
		//else
		//{
		//	output << grid[gridX_ - 1][j][(int)(gridZ_ / 2)] << '\n';
		//}
		output << grid[gridX_ - 1][j][(int)(gridZ_ / 2)] << '\n';
	}

	output.close();
	std::cout << "Potential saved" << std::endl;

	output.open(eFieldPath_);

	//for (size_t i = 1; i < gridX_ - i; i++)
	//{
	//	for (size_t j = 1; j < gridY_ - 2; j++)
	//	{
	//		if (std::abs(std::sqrtf(std::powf(electricField[i][j][(int)(gridZ_ / 2)][0], 2) + std::powf(electricField[i][j][(int)(gridZ_ / 2)][1], 2))) <= 0.001)
	//		{
	//			output << "\"{" << 0 << "," << 0 << "}\",";
	//		}
	//		else
	//		{
	//			output << "\"{" << electricField[i][j][(int)(gridZ_ / 2)][0] << "," << electricField[i][j][(int)(gridZ_ / 2)][1] << "}\",";
	//		}
	//	}

	//	if (std::abs(std::sqrtf(std::powf(electricField[i][gridY_-1][(int)(gridZ_ / 2)][0], 2) + std::powf(electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][1], 2))) <= 0.001)
	//	{
	//		output << "\"{" << 0 << "," << 0 << "}\"" << "\n";
	//	}
	//	else
	//	{
	//		output << "\"{" << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][0] << "," << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][1] << "}\"" << "\n";
	//	}
	//}

	for (size_t i = 1; i < gridX_ - 1; i++)
	{
		for (size_t j = 1; j < gridY_ - 2; j++)
		{
			output << "\"{" << electricField[i][j][(int)(gridZ_ / 2)][0] << "," << electricField[i][j][(int)(gridZ_ / 2)][1] << "}\",";
		}
			output << "\"{" << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][0] << "," << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][1] << "}\"" << "\n";
	}

	output.close();
	std::cout << "E-field saved" << std::endl;
}