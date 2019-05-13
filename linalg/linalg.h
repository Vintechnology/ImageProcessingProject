#pragma once
#include <math.h>
#include <vector>
#include <iostream>

namespace linalg {
	typedef std::vector<int> Vector;
	typedef std::vector<Vector> Matrix;

	void evaluate(const Matrix &matrix);

	std::string getVectorStr(const Vector &arr);
	std::string getMatrixStr(const Matrix &matrix);

	Vector shape(const Matrix &matrix);
	Vector shape(const Vector &vector);
	Matrix matmul(const Matrix &m1, const Matrix &m2);
	int dot(Vector vec1, Vector vec2);
	Matrix transpose(const Matrix &matrix);

	void convolution1D(const Matrix &src, Matrix &des, const Matrix &kernel);
	void convolution2D(const Matrix &src, Matrix &des, const Matrix &kernel);
}

using namespace linalg;

std::ostream& operator<< (std::ostream &os, const Matrix &matrix);
std::ostream& operator<< (std::ostream &os, const Vector &vec);

// ADDITION
Matrix operator+ (const Matrix &m1, const Matrix &m2);
Matrix operator+ (const Matrix &matrix, const Vector &vector);
Matrix operator+ (const Matrix &matrix, int a);

Matrix operator+ (const Vector &vector, const Matrix &matrix);
Vector operator+ (const Vector &vect1, const Vector &vect2);
Vector operator+ (const Vector &vector, int a);

// SUBTRACT
Matrix operator- (const Matrix &m1, const Matrix &m2);
Matrix operator- (const Matrix &matrix, const Vector &vector);
Matrix operator- (const Matrix &matrix, int a);

Matrix operator- (const Vector &vector, const Matrix &matrix);
Vector operator- (const Vector &vect1, const Vector &vect2);
Vector operator- (const Vector &vector, int a);

// MUL
Matrix operator* (const Matrix &m1, const Matrix &m2);
Matrix operator* (const Matrix &matrix, const Vector &vector);
Matrix operator* (const Matrix &matrix, int a);

Matrix operator* (const Vector &vector, const Matrix &matrix);
Vector operator* (const Vector &vect1, const Vector &vect2);
Vector operator* (const Vector &vector, int a);

// DIV
Matrix operator/ (const Matrix &m1, const Matrix &m2);
Matrix operator/ (const Matrix &matrix, const Vector &vector);
Matrix operator/ (const Matrix &matrix, int a);

Matrix operator/ (const Vector &vector, const Matrix &matrix);
Vector operator/ (const Vector &vect1, const Vector &vect2);
Vector operator/ (const Vector &vector, int a);