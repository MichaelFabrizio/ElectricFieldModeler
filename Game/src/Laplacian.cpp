#include "Laplacian.h"
#include <iostream>
#include <fstream>

#include <shlobj.h>
#include <windows.h>
#include <string>
#include <algorithm>
#pragma comment(lib, "shell32.lib")

Laplacian::Laplacian() : w(0.94f), threshold_(1e-6), path_(), electricField(),
gridConstructed_(false)
{
}

//Arguably the most important function in the program. This deletes the giant memory requirement that the program uses
Laplacian::~Laplacian()
{
	if (successful)
	{
		if (gridConstructed_)
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
	}
}

void Laplacian::Calculate(std::vector<std::unique_ptr<Shape>>& objectList)
{
	std::cout << "Calculating potential" << '\n';
	CheckOccupation(objectList);

	for (size_t b = 0; b < iterations_; b++)
	{
		for (int i = 1; i < gridX_-1; i++)
		{
			for (int j = 1; j < gridY_-1; j++)
			{
				for (int k = 1; k < gridZ_-1; k++)
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

				electricField[i][j][k][0] = -1.0*(grid[i + 1][j][k] - grid[i - 1][j][k]) * (gridScaling_ * 1000.0) / 2.0;
				electricField[i][j][k][1] = -1.0*(grid[i][j + 1][k] - grid[i][j - 1][k]) * (gridScaling_ * 1000.0) / 2.0;
				electricField[i][j][k][2] = -1.0*(grid[i][j][k + 1] - grid[i][j][k - 1]) * (gridScaling_ * 1000.0) / 2.0;
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


	//Admittedly this is code I'm not the most familiar with. I did my best to vet it. The primary function is to properly construct
	//The file path to be *User*/My documents      .   I don't full understand it, but it's some kind of Windows API code (filesystems etc)
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

	for (size_t j = 1; j < gridY_ - 1; j++)
	{
		for (size_t i = 1; i < gridX_ - 2; i++)
		{
			output << grid[i][j][(int)(gridZ_ / 2)] << ',';

		}
		output << grid[gridX_ - 1][j][(int)(gridZ_ / 2)] << '\n';
	}

	output.close();
	std::cout << "Potential saved" << std::endl;

	output.open(eFieldPath_);

	for (int i = 1; i < gridX_ - 1; i++)
	{
		for (int j = 1; j < gridY_ - 2; j++)
		{
			output << "\"{" << electricField[i][j][(int)(gridZ_ / 2)][0] << "," << electricField[i][j][(int)(gridZ_ / 2)][1] << "}\",";
		}
			output << "\"{" << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][0] << "," << electricField[i][gridY_ - 1][(int)(gridZ_ / 2)][1] << "}\"" << "\n";
	}

	output.close();
	std::cout << "E-field saved" << std::endl;
}

bool Laplacian::BuildGrid(int iterations, int gridX, int gridY, int gridZ, double gridScaling)
{
	gridConstructed_ = true;

	//Does not function right. If the grid isn't built then core has no idea what to do with itself
	if ((int)(8 * gridX * gridY * gridZ * gridScaling) > 8000000 || (int)gridScaling >= 4)
	{
		std::cout << "This grid would consume over 8gb of memory. Either the gridscaling or volumetric dimensions are too large." << '\n';
		return false;
	}
	else
	{

		iterations_ = iterations;
		gridX_ = (int)(gridX * gridScaling);
		gridY_ = (int)(gridY * gridScaling);
		gridZ_ = (int)(gridZ * gridScaling);
		gridScaling_ = gridScaling;

		grid = new double** [gridX_];
		occupiedInternal = new bool** [gridX_];
		electricField = new double*** [gridX_];

		for (size_t i = 0; i < gridX_; i++)
		{
			grid[i] = new double* [gridY_];

			for (size_t j = 0; j < gridY_; j++)
			{
				grid[i][j] = new double[gridZ_];
			}
		}

		for (size_t i = 0; i < gridX_; i++)
		{
			for (size_t j = 0; j < gridY_; j++)
			{
				for (size_t k = 0; k < gridZ_; k++)
				{
					grid[i][j][k] = 0;
				}
			}
		}

		for (size_t i = 0; i < gridX_; i++)
		{
			occupiedInternal[i] = new bool* [gridY_];

			for (size_t j = 0; j < gridY_; j++)
			{
				occupiedInternal[i][j] = new bool[gridZ_];
			}
		}

		for (size_t i = 0; i < gridX_; i++)
		{
			for (size_t j = 0; j < gridY_; j++)
			{
				for (size_t k = 0; k < gridZ_; k++)
				{
					occupiedInternal[i][j][k] = false;
				}
			}
		}
		//E-Field
		for (size_t i = 0; i < gridX_; i++)
		{
			electricField[i] = new double** [gridY_];

			for (size_t j = 0; j < gridY_; j++)
			{
				electricField[i][j] = new double* [gridZ_];
				for (size_t k = 0; k < gridZ_; k++)
				{
					electricField[i][j][k] = new double[3];
				}
			}
		}

		for (size_t i = 0; i < gridX_; i++)
		{
			for (size_t j = 0; j < gridY_; j++)
			{
				for (size_t k = 0; k < gridZ_; k++)
				{
					for (size_t w = 0; w < 3; w++)
					{
						electricField[i][j][k][w] = 0;
					}
				}
			}
		}
		return true;
	}
}

double****& Laplacian::GetEField()
{
	return electricField;
}

bool Laplacian::CheckEField()
{
	if (gridConstructed_)
		return true;
	else
		return false;
}

double Laplacian::GetGridScaling()
{
	return gridScaling_;
}