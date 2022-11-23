/*
 *   Copyright (c) 2007 John Weaver
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNMATRIX_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNMATRIX_H_

#include <initializer_list>
#include <cstdlib>
#include <ostream>

template<class T>
class HunMatrix {
 public:
  HunMatrix();
  HunMatrix(const size_t rows, const size_t columns);
  HunMatrix(const std::initializer_list<std::initializer_list<T>> init);
  HunMatrix(const HunMatrix<T> &other);
  HunMatrix<T> &operator=(const HunMatrix<T> &other);
  ~HunMatrix();
  // all operations modify the matrix in-place.
  void resize(const size_t rows, const size_t columns, const T default_value = 0);
  void clear();
  T &operator()(const size_t x, const size_t y);
  const T &operator()(const size_t x, const size_t y) const;
  const T min() const;
  const T max() const;
  inline size_t minsize() { return ((m_rows < m_columns) ? m_rows : m_columns); }
  inline size_t columns() const { return m_columns; }
  inline size_t rows() const { return m_rows; }

  friend std::ostream &operator<<(std::ostream &os, const HunMatrix &matrix) {
    os << "HunMatrix:" << std::endl;
    for (size_t row = 0; row < matrix.rows(); row++) {
      for (size_t col = 0; col < matrix.columns(); col++) {
        os.width(8);
        os << matrix(row, col) << ",";
      }
      os << std::endl;
    }
    return os;
  }

 private:
  T **m_matrix;
  size_t m_rows;
  size_t m_columns;
};
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNMATRIX_H_

