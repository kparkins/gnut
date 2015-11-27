#ifndef GFX_GRID_H
#define GFX_GRID_H

#include <stddef.h>
#include <string.h>

namespace gfx {
    template<typename T>
    class grid {

    public:

        grid(size_t rows, size_t cols);
        ~grid();

        T* operator[](int row);

    protected:

        T* m_data;
        size_t m_rows;
        size_t m_cols;

    };
}

template <typename T>
gfx::grid<T>::grid(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
    m_data = new T[rows * cols];
    memset(static_cast<void*>(&m_data[0]), 0, m_rows * m_cols * sizeof(T));
}

template <typename T>
gfx::grid<T>::~grid() {
    delete [] m_data;
}

template <typename T>
T* gfx::grid<T>::operator[](int row) {
    return &m_data[row * m_rows];
}

#endif