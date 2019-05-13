#include "linalg.h"
#include <sstream>

namespace linalg {
	std::string errorLog(const Matrix &matrix, int rowidx) {
		std::string log = "Illegal matrix creation: Matrix rows should be at same size\n";
		std::stringstream ss;

		ss << log << "\n[";
		int size = matrix.size();
		for (int i = 0; i < size; i++) {
			if (i != 0)
				ss << " ";
			ss << getVectorStr(matrix[i]);
			if (i < size - 1) {
				ss << ",";
			}
			else
				ss << "]";
			if (i == rowidx)
				ss << " <-";
			ss << "\n";
		}
		return ss.str();
	}

	void evaluate(const Matrix &matrix) {
		if (matrix.empty())
			throw std::runtime_error("Empty matrix");

		int col = matrix[0].size();
		for (int i = 1; i < (int)(matrix.size()); i++) {
			int rowSize = (int)(matrix[i].size());
			if (rowSize != col) {
				throw std::runtime_error(errorLog(matrix, i));
			}
		}

		if (matrix[0].empty())
			throw std::runtime_error("Empty matrix elements");
	}

	void evaluate(const Vector &vector) {
		if (vector.empty())
			throw std::runtime_error("Empty vector");
	}

	std::string getVectorStr(const Vector & arr)
	{
		std::stringstream ss;
		ss << "[";
		int size = arr.size();
		for (int i = 0; i < size; i++) {
			if (arr[i] >= 0)
				ss << " ";
			ss << arr[i];
			if (i < size - 1) {
				ss << ",";
			}
		}
		ss << " ]";
		return ss.str();
	}

	std::string getMatrixStr(const Matrix & matrix)
	{
		std::stringstream ss;
		ss << "\n[";
		int size = matrix.size();
		for (int i = 0; i < size; i++) {
			if (i != 0)
				ss << " ";
			ss << getVectorStr(matrix[i]);
			if (i < size - 1) {
				ss << ",\n";
			}
		}
		ss << "]\n";
		return ss.str();
	}

	Vector shape(const Matrix & matrix)
	{
		return { (int)matrix.size(), (int)(matrix.size() != 0 ? matrix[0].size() : 0) };
	}

	Vector shape(const Vector & vector)
	{
		return { (int)vector.size() };
	}


	Matrix matmul(const Matrix & m1, const Matrix & m2)
	{
		evaluate(m1);
		evaluate(m2);
		if (m1[0].size() != m2.size()) {
			std::stringstream ss;
			ss << "Cannot do matrix multiply with matrix1 columns = " << m1[0].size() << " matrix2 rows = " << m2.size();
			throw std::runtime_error(ss.str());
		}
		int rows = m1.size();
		int cols = m2[0].size();
		int inner = m2.size();
		Matrix result(rows, Vector(cols));

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result[i][j] = 0;
				for (int k = 0; k < inner; k++) {
					result[i][j] += m1[i][k] * m2[k][j];
				}
			}
		}

		return result;
	}

	int dot(Vector vec1, Vector vec2)
	{
		if (vec1.size() != vec2.size()) {
			std::stringstream ss;
			ss << "Cannot do dot product with different size vector vector1 = " << vec1.size() << " vector2 = " << vec2.size();
			throw std::runtime_error(ss.str());
		}
		int result = 0;
		for (int i = 0; i < (int)vec1.size(); i++) {
			result += vec1[i] * vec2[i];
		}
		return result;
	}

	Matrix transpose(const Matrix & matrix)
	{
		evaluate(matrix);
		Vector size = shape(matrix);
		Matrix result(size[1], Vector(size[0]));

		for (int row = 0; row < size[0]; row++) {
			for (int col = 0; col < size[1]; col++) {
				result[col][row] = matrix[row][col];
			}
		}
		return result;
	}

	int checkConvolution1D(const Vector &srcShape, const Vector &desShape, const Vector &kernelShape)
	{
		if (kernelShape[0] != 1 && kernelShape[1] != 1) {
			throw  std::runtime_error("Cannot perform 1D convolution with 2D kernel");
		}
		int axis = (kernelShape[0] != 1 ? 0 : 1);
		if (kernelShape[axis] % 2 == 0)
			throw  std::runtime_error("Kernel size must be an odd number");
		if (kernelShape[axis] > srcShape[axis])
			throw  std::runtime_error("Kernel is to large to perform convolution");

		if (srcShape[0] != desShape[0] || srcShape[1] != desShape[1]) {
			throw  std::runtime_error("Source and Destination must be same size");
		}
		return axis;
	}

	void convolution1D(const Matrix & src, Matrix & des, const Matrix & kernel)
	{
		evaluate(src);
		evaluate(des);
		evaluate(kernel);
		Vector srcShape = shape(src);
		Vector desShape = shape(des);
		Vector kernelShape = shape(kernel);
		int axis = checkConvolution1D(srcShape, desShape, kernelShape);
		int halfK = kernelShape[axis] / 2;
		int maxAxis = srcShape[axis] - 1;

		for (int desRow = 0; desRow < desShape[0]; desRow++)
		{
			for (int desCol = 0; desCol < desShape[1]; desCol++)
			{

				int result = 0;
				for (int k = 0; k < kernelShape[axis]; k++)
				{
					int index;
					if (axis == 0) {
						index = desRow - halfK + k;
					}
					else {
						index = desCol - halfK + k;
					}

					if (index<0 || index>maxAxis)
						continue;

					result += (axis == 0 ? kernel[k][0] * src[index][desCol] : kernel[0][k] * src[desRow][index]);
				}
				des[desRow][desCol] = result;

			}
		}
	}
	/*
		des is the result of the convolution which has the same size with src
	*/

	void checkConvolution2D(const Vector &srcShape, const Vector &desShape, const Vector &kernelShape)
	{
		if (srcShape[0] < kernelShape[0] || srcShape[1] < kernelShape[1]) {
			throw  std::runtime_error("Kernel to large to perform convolution");
		}
		else if (kernelShape[0] != kernelShape[1]) {
			throw  std::runtime_error("Kernel must be square matrix");
		}
		else if (kernelShape[0] % 2 == 0) {
			throw  std::runtime_error("Kernel size must be an odd number");
		}

		if (srcShape[0] != desShape[0] || srcShape[1] != desShape[1]) {
			throw  std::runtime_error("Source and Destination must be same size");
		}
	}

	void convolution2D(const Matrix & src, Matrix & des, const Matrix & kernel)
	{
		evaluate(src);
		evaluate(des);
		evaluate(kernel);
		Vector shapeSrc = shape(src);
		Vector shapeKernel = shape(kernel);
		Vector shapeDes = shape(des);
		checkConvolution2D(shapeSrc, shapeDes, shapeKernel);

		int halfK = shapeKernel[0] / 2;
		int maxRow = shapeSrc[0] - 1;
		int maxCol = shapeSrc[1] - 1;

		for (int desRow = 0; desRow < shapeSrc[0]; desRow++)
		{
			for (int desCol = 0; desCol < shapeSrc[1]; desCol++)
			{

				int result = 0;
				for (int kRow = 0; kRow < shapeKernel[0]; kRow++)
				{
					for (int kCol = 0; kCol < shapeKernel[1]; kCol++)
					{
						int srcRow = desRow - halfK + kRow;
						int srcCol = desCol - halfK + kCol;
						if (srcRow<0 || srcCol<0 || srcRow>maxRow || srcCol>maxCol) {
							continue;
						}
						result += src[srcRow][srcCol] * kernel[kRow][kCol];

					}
				}
				des[desRow][desCol] = result;

			}
		}
	}
}

using namespace linalg;

std::ostream& operator<<(std::ostream & os, const Matrix & matrix)
{
	os << getMatrixStr(matrix);
	return os;
}

std::ostream & operator<<(std::ostream & os, const Vector & vec)
{
	os << getVectorStr(vec);
	return os;
}

Matrix operator+(const Matrix & m1, const Matrix & m2)
{
	evaluate(m1);
	evaluate(m2);
	Vector shapeM1 = shape(m1);
	Vector shapeM2 = shape(m2);
	// check broadcasting
	// If a row vector
	// If a column vector
	// Normal element wise addition
	if (shapeM1[0] != shapeM2[0] && shapeM1[1] != shapeM2[1]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM1 << "and" << shapeM2;
		throw std::runtime_error(ss.str());
	}
	else if (shapeM1[0] != 1 && shapeM1[1] != 1 && shapeM2[0] != 1 && shapeM2[1] != 1) {
		throw std::runtime_error("Cannot perform broadcasting without a row or column matrix");
	}
	int maxX = (shapeM1[0] > shapeM2[0] ? shapeM1[0] : shapeM2[0]);
	int maxY = (shapeM1[1] > shapeM2[1] ? shapeM1[1] : shapeM2[1]);
	Matrix result(maxX, Vector(maxY));
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			result[i][j] = m1[i%shapeM1[0]][j%shapeM1[1]] + m2[i%shapeM2[0]][j%shapeM2[1]];
		}
	}
	return result;
}

Matrix operator+(const Matrix & matrix, const Vector & vector)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] + vector[i];
		}
	}
	return result;
}

Matrix operator+(const Matrix & matrix, int a)
{
	evaluate(matrix);
	Vector shapeM = shape(matrix);

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] + a;
		}
	}
	return result;
}

Matrix operator+(const Vector & vector, const Matrix & matrix)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] + vector[i];
		}
	}
	return result;
}

Vector operator+(const Vector & vect1, const Vector & vect2)
{
	evaluate(vect1);
	evaluate(vect2);
	Vector shapeV1 = shape(vect1);
	Vector shapeV2 = shape(vect2);
	if (shapeV1[0] != shapeV2[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with vector size " << shapeV1 << "and" << shapeV2;
		throw std::runtime_error(ss.str());
	}
	Vector result(shapeV1[0]);
	for (int i = 0; i < shapeV1[0]; i++) {
		result[i] = vect1[i] + vect2[i];
	}
	return result;
}

Vector operator+(const Vector & vector, int a)
{
	evaluate(vector);
	Vector shapeV1 = shape(vector);
	Vector result(shapeV1[0]);
	for (int i = 0; i < shapeV1[0]; i++) {
		result[i] = vector[i] + a;
	}
	return result;
}

Matrix operator-(const Matrix & m1, const Matrix & m2)
{
	evaluate(m1);
	evaluate(m2);
	Vector shapeM1 = shape(m1);
	Vector shapeM2 = shape(m2);
	// check broadcasting
	// If a row vector
	// If a column vector
	// Normal element wise addition
	if (shapeM1[0] != shapeM2[0] && shapeM1[1] != shapeM2[1]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM1 << "and" << shapeM2;
		throw std::runtime_error(ss.str());
	}
	else if (shapeM1[0] != 1 && shapeM1[1] != 1 && shapeM2[0] != 1 && shapeM2[1] != 1) {
		throw std::runtime_error("Cannot perform broadcasting without a row or column matrix");
	}
	int maxX = (shapeM1[0] > shapeM2[0] ? shapeM1[0] : shapeM2[0]);
	int maxY = (shapeM1[1] > shapeM2[1] ? shapeM1[1] : shapeM2[1]);
	Matrix result(maxX, Vector(maxY));
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			result[i][j] = m1[i%shapeM1[0]][j%shapeM1[1]] - m2[i%shapeM2[0]][j%shapeM2[1]];
		}
	}
	return result;
}

Matrix operator-(const Matrix & matrix, const Vector & vector)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] - vector[i];
		}
	}
	return result;
}

Matrix operator-(const Matrix & matrix, int a)
{
	evaluate(matrix);
	Vector shapeM = shape(matrix);

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] - a;
		}
	}
	return result;
}

Matrix operator-(const Vector & vector, const Matrix & matrix)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = vector[i] - matrix[i][j];
		}
	}
	return result;
}

Vector operator-(const Vector & vect1, const Vector & vect2)
{
	evaluate(vect1);
	evaluate(vect2);
	Vector shapeV1 = shape(vect1);
	Vector shapeV2 = shape(vect2);
	if (shapeV1[0] != shapeV2[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with vector size " << shapeV1 << "and" << shapeV2;
		throw std::runtime_error(ss.str());
	}
	Vector result(shapeV1[0]);
	for (int i = 0; i < shapeV1[0]; i++) {
		result[i] = vect1[i] - vect2[i];
	}
	return result;
}

Vector operator-(const Vector & vector, int a)
{
	evaluate(vector);
	Vector shapeV = shape(vector);

	Vector result(shapeV[0]);
	for (int i = 0; i < shapeV[0]; i++) {
		result[i] = vector[i] - a;
	}
	return result;
}

Matrix operator*(const Matrix & m1, const Matrix & m2)
{
	evaluate(m1);
	evaluate(m2);
	Vector shapeM1 = shape(m1);
	Vector shapeM2 = shape(m2);
	// check broadcasting
	// If a row vector
	// If a column vector
	// Normal element wise addition
	if (shapeM1[0] != shapeM2[0] && shapeM1[1] != shapeM2[1]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM1 << "and" << shapeM2;
		throw std::runtime_error(ss.str());
	}
	else if (shapeM1[0] != 1 && shapeM1[1] != 1 && shapeM2[0] != 1 && shapeM2[1] != 1) {
		throw std::runtime_error("Cannot perform broadcasting without a row or column matrix");
	}
	int maxX = (shapeM1[0] > shapeM2[0] ? shapeM1[0] : shapeM2[0]);
	int maxY = (shapeM1[1] > shapeM2[1] ? shapeM1[1] : shapeM2[1]);
	Matrix result(maxX, Vector(maxY));
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			result[i][j] = m1[i%shapeM1[0]][j%shapeM1[1]] * m2[i%shapeM2[0]][j%shapeM2[1]];
		}
	}
	return result;
}

Matrix operator*(const Matrix & matrix, const Vector & vector)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] * vector[i];
		}
	}
	return result;
}

Matrix operator*(const Matrix & matrix, int a)
{
	evaluate(matrix);
	Vector shapeM = shape(matrix);

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] * a;
		}
	}
	return result;
}

Matrix operator*(const Vector & vector, const Matrix & matrix)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] * vector[i];
		}
	}
	return result;
}

Vector operator*(const Vector & vect1, const Vector & vect2)
{
	evaluate(vect1);
	evaluate(vect2);
	Vector shapeV1 = shape(vect1);
	Vector shapeV2 = shape(vect2);
	if (shapeV1[0] != shapeV2[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with vector size " << shapeV1 << "and" << shapeV2;
		throw std::runtime_error(ss.str());
	}
	Vector result(shapeV1[0]);
	for (int i = 0; i < shapeV1[0]; i++) {
		result[i] = vect1[i] * vect2[i];
	}
	return result;
}

Vector operator*(const Vector & vector, int a)
{
	evaluate(vector);
	Vector shapeV = shape(vector);
	Vector result(shapeV[0]);
	for (int i = 0; i < shapeV[0]; i++) {
		result[i] = vector[i] * a;
	}
	return result;
}

Matrix operator/(const Matrix & m1, const Matrix & m2)
{
	evaluate(m1);
	evaluate(m2);
	Vector shapeM1 = shape(m1);
	Vector shapeM2 = shape(m2);
	// check broadcasting
	// If a row vector
	// If a column vector
	// Normal element wise addition
	if (shapeM1[0] != shapeM2[0] && shapeM1[1] != shapeM2[1]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM1 << "and" << shapeM2;
		throw std::runtime_error(ss.str());
	}
	else if (shapeM1[0] != 1 && shapeM1[1] != 1 && shapeM2[0] != 1 && shapeM2[1] != 1) {
		throw std::runtime_error("Cannot perform broadcasting without a row or column matrix");
	}
	int maxX = (shapeM1[0] > shapeM2[0] ? shapeM1[0] : shapeM2[0]);
	int maxY = (shapeM1[1] > shapeM2[1] ? shapeM1[1] : shapeM2[1]);
	Matrix result(maxX, Vector(maxY));
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			result[i][j] = m1[i%shapeM1[0]][j%shapeM1[1]] / m2[i%shapeM2[0]][j%shapeM2[1]];
		}
	}
	return result;
}

Matrix operator/(const Matrix & matrix, const Vector & vector)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] / vector[i];
		}
	}
	return result;
}

Matrix operator/(const Matrix & matrix, int a)
{
	evaluate(matrix);
	Vector shapeM = shape(matrix);

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] / a;
		}
	}
	return result;
}

Matrix operator/(const Vector & vector, const Matrix & matrix)
{
	evaluate(matrix);
	evaluate(vector);
	Vector shapeM = shape(matrix);
	Vector shapeV = shape(vector);
	if (shapeM[0] != shapeV[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with matrix size " << shapeM << "and vector size" << shapeV;
		throw std::runtime_error(ss.str());
	}

	Matrix result(shapeM[0], Vector(shapeM[1]));
	for (int i = 0; i < shapeM[0]; i++) {
		for (int j = 0; j < shapeM[1]; j++) {
			result[i][j] = matrix[i][j] / vector[i];
		}
	}
	return result;
}

Vector operator/(const Vector & vect1, const Vector & vect2)
{
	evaluate(vect1);
	evaluate(vect2);
	Vector shapeV1 = shape(vect1);
	Vector shapeV2 = shape(vect2);
	if (shapeV1[0] != shapeV2[0]) {
		std::stringstream ss;
		ss << "Cannot perform addition operation with vector size " << shapeV1 << "and" << shapeV2;
		throw std::runtime_error(ss.str());
	}
	Vector result(shapeV1[0]);
	for (int i = 0; i < shapeV1[0]; i++) {
		result[i] = vect1[i] / vect2[i];
	}
	return result;
}

Vector operator/(const Vector & vector, int a)
{
	evaluate(vector);
	Vector shapeV = shape(vector);
	Vector result(shapeV[0]);
	for (int i = 0; i < shapeV[0]; i++) {
		result[i] = vector[i] / a;
	}
	return result;
}
