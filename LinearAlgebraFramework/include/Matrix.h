#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdexcept>

/*
Use hash table to pull in information better or stick with vector, for right now.
*/

template<class T>
class Matrix{
public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, const std::vector<T>& input_data);
    Matrix(const Matrix<T>& _matrix);

    ~Matrix();

    // Configuration methods.
    bool resize(int row, int col);
    void setToIdentity(); // New Function

    // Methods to access elements in the matrix's
    // Which accept row and col indices.
    T getValue(int row, int col);
    bool setValue(int row, int col, T val);
    int getRows() const;
    int getCols() const;

    bool inverse(); // New Function
    bool compare(const Matrix<T>& martx, T tolerance){
		return false;
    }

    bool operator==(const Matrix<T>& rhs) const;

    // operator overloading for addition, subtraction, and multiplication.
    // REVIEW NOTE: Declared with "friend" keyword, so these operator overloading can access the private variables, or instances.
    // Even if they are not going to be defined as within this class.
    template<class U> friend Matrix<U> operator+ (const Matrix<U>& lhs, const Matrix<U>& rhs); // Add Matrix + Matrix
    template<class U> friend Matrix<U> operator+ (const U& lhs, const Matrix<U>& rhs); // Add Scalar + Matrix
    template<class U> friend Matrix<U> operator+ (const Matrix<U>& lhs, const U& rhs); // Add Matrix + Scalar

    template<class U> friend Matrix<U> operator- (const Matrix<U>& lhs, const Matrix<U>& rhs); // Subtract Matrix - Matrix
    template<class U> friend Matrix<U> operator- (const U& lhs, const Matrix<U>& rhs); // Subtract Scalar - Matrix
    template<class U>friend Matrix<U> operator- (const Matrix<U>& lhs, const U& rhs); // Subtract Matrix - Scalar

    template<class U> friend Matrix<U> operator* (const Matrix<U>& lhs, const Matrix<U>& rhs); // Multiply Matrix * Matrix
    template<class U> friend Matrix<U> operator* (const U& lhs, const Matrix<U>& rhs); // Multiply Scalar * Matrix
    template<class U> friend Matrix<U> operator* (const Matrix<U>& lhs, const U& rhs); // Multiply Matrix * Scalar

private:
    // Converts subscripts into an index.
    int sub2Index(int row, int col);
private:
    std::vector<T> data; //Matrix data
    int rows, cols;
    size_t size;
};

/* ********************************************************************************* */
/* *************************** DEFINING FUNCTIONS HERE ***************************** */
/* ********************************************************************************* */

template<class T>
Matrix<T>::Matrix(){
    rows = 1;
    cols = 1;
    size = 1;
    data = std::vector<T>(size);
    data.at(0) = 0.0;
}

template<class T>
Matrix<T>::Matrix(int rows, int cols){
    this->rows = rows;
    this->cols = cols;
    this->size = this->rows  * this->cols;
    data = std::vector<T>(this->size);

    for(size_t i = 0; i < data.size(); i++)
        data.at(i) = 0.0;
}

template<class T>
Matrix<T>::Matrix(int rows, int cols, const std::vector<T>& input_data){
    this->rows = rows;
    this->cols = cols;
    this->size = this->rows  * this->cols;
    data = std::vector<T>(this->size);

    for(size_t i = 0; i < data.size(); i++)
        data.at(i) = input_data.at(i);
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& other){
    this->rows = other.rows;
    this->cols = other.cols;
    this->size = other.size;
    this->data = std::vector<T>(this->size);

    for(size_t i = 0; i < this->data.size(); i++)
        data.at(i) = other.data.at(i);
}

template<class T>
Matrix<T>::~Matrix(){ }

template<class T>
bool Matrix<T>::resize(int rows, int cols){
    this->rows = rows;
    this->cols = cols;
    this->size = (this->rows * this->cols);

    if(!data.size()){
        for(size_t i = 0; i < this->size; i++)
            data.at(i) = 0.0;
        return true;
    }
    return false;
}

/* ********************************************************************************* */
/* *************************** ELEMENTS FUNCTIONS HERE ***************************** */
/* ********************************************************************************* */

template<class T>
T Matrix<T>::getValue(int row, int col){
    int linearIndex = sub2Index(row, col);
    return ((linearIndex >= 0) ? data.at(linearIndex) : 0.0);
}

template<class T>
bool Matrix<T>::setValue(int row, int col, T val){
    int linearIndex = sub2Index(row, col);

    if(linearIndex >= 0){
        data.at(linearIndex) = val;
        return true;
    }

    // return ((linearIndex >= 0) ? data.at(linearIndex) : 0.0);
    return false;
}

template<class T>
int Matrix<T>::getRows() const { return this->rows; }

template<class T>
int Matrix<T>::getCols() const { return this->cols; }


/* ********************************************************************************* */
/* ******************* OPERATOR OVERLOADING + FUNCTIONS HERE *********************** */
/* ********************************************************************************* */

template<class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs){
    T rows = lhs.rows;
    T cols = lhs.cols;
    T size = lhs.size;

    std::vector<T> output = std::vector<T>(size);

    // Iterating through the elements in matrix
    // Then adding matrix from left side to the right side of the matrix.
    // Afterwards, storing into the temp_output variable.
    for(size_t i = 0; i < output.size(); i++)
        output.at(i) = lhs.matrix_data.at(i) + rhs.matrix_data.at(i);

    // After addition operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.

    Matrix<T> retVal(rows, cols, output);
}

// Addition: SCALAR + MATRIX
template<class T>
Matrix<T> operator+(const T& lhs, const Matrix<T>& rhs){
    int rows = rhs.rows;
    int cols = rhs.cols;
    size_t size = rhs.size;

    // T* temp_output = new T[_element_size];
    std::vector<T> output = std::vector<T>(rows*cols);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs + rhs.data.at(i);

    Matrix<T> result (rows, cols, output);
    return result;
}


// Addition: MATRIX + SCALAR
template<class T>
Matrix<T> operator+(const Matrix<T>& lhs, const T& rhs){
    int _rows = lhs.rows;
    int _cols = lhs.cols;
    size_t size = lhs.size;

    std::vector<T> output = std::vector<T>(size);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs.data.at(i) + rhs;
    
    Matrix<T> result(_rows, _cols, output);
    return result;
}


/* ********************************************************************************* */
/* ******************* OPERATOR OVERLOADING - FUNCTIONS HERE *********************** */
/* ********************************************************************************* */


// Subtracting: MATRIX - MATRIX
template <class T>
Matrix<T> operator- (const Matrix<T>& lhs, const Matrix<T>& rhs){
    int _rows = lhs.rows;
    int _cols = lhs.cols;
    size_t size = lhs.size;

    std::vector<T> output = std::vector<T>(size);

    // Iterating through the elements in matrix
    // Then adding matrix from left side to the right side of the matrix.
    // Afterwards, storing into the temp_output variable.
    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs.data.at(i) - rhs.data.at(i);

    // After subtracting operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.
    Matrix<T> result(_rows, _cols, output);
    
    return result;
}


//Subtracting: SCALAR - MATRIX
template <class T>
Matrix<T> operator- (const T& lhs, const Matrix<T>& rhs){
    int _rows = rhs.rows;
    int _cols = rhs.cols;
    size_t size = rhs.size;

    std::vector<T> output = std::vector<T>(size);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs - rhs.data.at(i);
    
    // After addition operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.
    Matrix<T> result(_rows, _cols, output);
    
    return result;
}


// Subtracting: MATRIX - SCALAR
template <class T>
Matrix<T> operator- (const Matrix<T>& lhs, const T& rhs){
    int _rows = lhs.rows;
    int _cols = lhs.cols;
    size_t size = lhs.size;

    std::vector<T> output = std::vector<T>(size);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs.data.at(i) - rhs;
    
    // After addition operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.
    Matrix<T> result(_rows, _cols, output);
    
    return result;
}

/* ********************************************************************************* */
/* ******************* OPERATOR OVERLOADING * FUNCTIONS HERE *********************** */
/* ********************************************************************************* */


// Multiplication: MATRIX * MATRIX
template<class T> Matrix<T> operator* (const Matrix<T>& lhs, const Matrix<T>& rhs){
    // left {row,col}
    // right {row, col}
    int r_rows = rhs.rows;
    int r_cols = rhs.cols;
    int l_rows = lhs.rows;
    int l_cols = lhs.cols;

    bool row_validation = (l_cols == r_rows);
    
    if(row_validation){
        // T *temp_output = new T[lhs.rows * rhs.cols];
        int size = lhs.rows * rhs.cols;
        // std::vector<T> output = std::vector<T>(left_size);
        std::vector<T> output(size);

        // std::cerr << "Point 1: Checking out of range!" << std::endl;

        // Looping through each row on the left hand side.
        /*
        i -> left_row = 0;
        j -> right_col = 0;
        k -> left_col = 0;
        */
        for(int i = 0; i < l_rows; i++){
            for(int j = 0; j < r_cols; j++){ // Looping through each column on the Right hand side.
                T element_output = 0.0; // To contain and get the element output.

                // std::cerr << "Point 2: Checking out of range!" << std::endl;
                
                for(int k = 0; k < l_cols; k++){ // Iterate through the elements in left hand side row.
                    // Computing the right linear index
                    int left_linearIndex = (i * l_cols) + k;

                    // computing the right linear index
                    int right_linearIndex = (k * r_cols) + j;

                    // Getting the element out put based by multiplying the matrices through here!
                    element_output += (lhs.data.at(left_linearIndex) * rhs.data.at(right_linearIndex));
                }

                // Storing/Containing into output
                int linearIndex_output = (i * r_cols) + j;
                output.at(linearIndex_output) = element_output;
            }
        }
        // Then storing that temp_output into the result, before deleting temp_output.
        Matrix result(l_rows, r_cols, output);
        return result;
    }
    else{
        Matrix<T> result_output(1,1);
        std::cout << "This executes for multiplication in this overloader operator*" << std::endl;
        return result_output;
    }
}

// Multiplicatoin: SCALAR * MULTIPLICATION
template<class T>
Matrix<T> operator* (const T& lhs, const Matrix<T>& rhs){
    int rows = rhs.rows;
    int cols = rhs.cols;
    size_t size = rhs.size;

    std::vector<T> output = std::vector<T>(size);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs * rhs.data.at(i);
    
    // After addition operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.
    Matrix<T> result(rows, cols, output);
    
    return result;
}


// Multiplication: MATRIX * SCALAR
template<class T> Matrix<T> operator* (const Matrix<T>& lhs, const T& rhs){
    int _rows = lhs.rows;
    int _cols = lhs.cols;
    size_t size = lhs.size;

    std::vector<T> output = std::vector<T>(size);

    for(size_t i = 0; i < size; i++)
        output.at(i) = lhs.data.at(i) * rhs;
    
    // After addition operations, then add into a new instance of matrix,
    // to delete temp_output after storing in the new matrix instance.
    // Then returning that new matrix object that we newly created.
    Matrix<T> result(_rows, _cols, output);
    
    return result;
}

/* ********************************************************************************* */
/* ******************* OPERATOR OVERLOADING == FUNCTIONS HERE ********************** */
/* ********************************************************************************* */
// Matrix<T> == Matrix<T>
template<class T>
bool Matrix<T>::operator==(const Matrix<T>& rhs)const{
    
    // Checking if matrices are the same size, if not return false.
    bool row_validation = (this->rows != rhs.rows);
    bool col_validation = (this->cols != rhs.cols);

    if(row_validation and col_validation) return false;

    // Checking if the elements are equal to each other
    bool flagged = true;

    for(size_t i = 0; i < this->size; i++){

        // Checking if the data in the matrices are equal to one another, here
        bool data_validation = (this->data.at(i) != rhs.data.at(i));
        if(data_validation)
        flagged = false;
    }

    return flagged;
}


/* ********************************************************************************* */
/* **************************** PRIVATE FUNCTIONS HERE ***************************** */
/* ********************************************************************************* */

template<class T>
int Matrix<T>::sub2Index(int row, int col){
    // For checking for validation for rows and cols
    bool row_validation = (row < this->rows && row >= 0);
    bool col_validation = (col < this->cols && col >= 0);

    if(row_validation && col_validation){
        return (row * this->cols) + col;
    }
    return -1;
}
