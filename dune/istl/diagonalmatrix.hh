// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_DIAGONAL_MATRIX_NEW_HH
#define DUNE_DIAGONAL_MATRIX_NEW_HH

/*! \file
   \brief  This file implements a quadratic matrix of fixed size which is diagonal.
 */

#include <cmath>
#include <cstddef>
#include <complex>
#include <iostream>
#include <memory>
#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/genericiterator.hh>



namespace Dune {

  template< class K, int n > class DiagonalRowVectorConst;
  template< class K, int n > class DiagonalRowVector;
  template< class DiagonalMatrixType > class DiagonalMatrixWrapper;
  template< class C, class T, class R> class ContainerWrapperIterator;

  /**
      @addtogroup DenseMatVec
      @{
   */

  /**
     *@brief A diagonal matrix of static size.
   * This is meant to be a replacement of FieldMatrix for the case that
   * it is a diagonal matrix.
   */
  template<class K, int n>
  class DiagonalMatrix
  {
    typedef DiagonalMatrixWrapper< DiagonalMatrix<K,n> > WrapperType;

  public:
    //===== type definitions and constants

    //! export the type representing the field
    typedef K field_type;

    //! export the type representing the components
    typedef K block_type;

    //! The type used for the index access and size operations.
    typedef std::size_t size_type;

    //! We are at the leaf of the block recursion
    enum {
      //! The number of block levels we contain. This is 1.
      blocklevel = 1
    };

    //! Each row is implemented by a field vector
    typedef DiagonalRowVector<K,n> row_type;
    typedef row_type reference;
    typedef DiagonalRowVectorConst<K,n> const_row_type;
    typedef const_row_type const_reference;

    //! export size
    enum {
      //! The number of rows.
      rows = n,
      //! The number of columns.
      cols = n
    };



    //===== constructors

    //! Default constructor
    DiagonalMatrix () {}

    //! Constructor initializing the whole matrix with a scalar
    DiagonalMatrix (const K& k)
      : diag_(k)
    {}

    //! Constructor initializing the diagonal with a vector
    DiagonalMatrix (const FieldVector<K,n>& diag)
      : diag_(diag)
    {}


    //===== assignment from scalar
    DiagonalMatrix& operator= (const K& k)
    {
      diag_ = k;
      return *this;
    }

    // check if matrix is identical to other matrix (not only identical values)
    bool identical(const DiagonalMatrix<K,n>& other) const
    {
      return (this==&other);
    }

    //===== iterator interface to rows of the matrix
    //! Iterator class for sequential access
    typedef ContainerWrapperIterator<const WrapperType, reference, reference> Iterator;
    //! typedef for stl compliant access
    typedef Iterator iterator;
    //! rename the iterators for easier access
    typedef Iterator RowIterator;
    //! rename the iterators for easier access
    typedef typename row_type::Iterator ColIterator;

    //! begin iterator
    Iterator begin ()
    {
      return Iterator(WrapperType(this),0);
    }

    //! end iterator
    Iterator end ()
    {
      return Iterator(WrapperType(this),n);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeEnd
    //! instead.
    Iterator rbegin() DUNE_DEPRECATED
    {
      return beforeEnd();
    }

    //! @returns an iterator that is positioned before
    //! the end iterator of the rows, i.e. at the last row.
    Iterator beforeEnd ()
    {
      return Iterator(WrapperType(this),n-1);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeBegin
    //! instead.
    Iterator rend ()  DUNE_DEPRECATED
    {
      return beforeBegin();
    }

    //! @returns an iterator that is positioned before
    //! the first row of the matrix.
    Iterator beforeBegin ()
    {
      return Iterator(WrapperType(this),-1);
    }


    //! Iterator class for sequential access
    typedef ContainerWrapperIterator<const WrapperType, const_reference, const_reference> ConstIterator;
    //! typedef for stl compliant access
    typedef ConstIterator const_iterator;
    //! rename the iterators for easier access
    typedef ConstIterator ConstRowIterator;
    //! rename the iterators for easier access
    typedef typename const_row_type::ConstIterator ConstColIterator;

    //! begin iterator
    ConstIterator begin () const
    {
      return ConstIterator(WrapperType(this),0);
    }

    //! end iterator
    ConstIterator end () const
    {
      return ConstIterator(WrapperType(this),n);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeEnd
    //! instead.
    ConstIterator rbegin() const DUNE_DEPRECATED
    {
      return beforeEnd();
    }

    //! @returns an iterator that is positioned before
    //! the end iterator of the rows. i.e. at the last row.
    ConstIterator beforeEnd() const
    {
      return ConstIterator(WrapperType(this),n-1);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeBegin
    //! instead.
    ConstIterator rend () const DUNE_DEPRECATED
    {
      return beforeBegin();
    }

    //! @returns an iterator that is positioned before
    //! the first rowof the matrix.
    ConstIterator beforeBegin () const
    {
      return ConstIterator(WrapperType(this),-1);
    }



    //===== vector space arithmetic

    //! vector space addition
    DiagonalMatrix& operator+= (const DiagonalMatrix& y)
    {
      diag_ += y.diag_;
      return *this;
    }

    //! vector space subtraction
    DiagonalMatrix& operator-= (const DiagonalMatrix& y)
    {
      diag_ -= y.diag_;
      return *this;
    }

    //! vector space multiplication with scalar
    DiagonalMatrix& operator+= (const K& k)
    {
      diag_ += k;
      return *this;
    }

    //! vector space division by scalar
    DiagonalMatrix& operator-= (const K& k)
    {
      diag_ -= k;
      return *this;
    }

    //! vector space multiplication with scalar
    DiagonalMatrix& operator*= (const K& k)
    {
      diag_ *= k;
      return *this;
    }

    //! vector space division by scalar
    DiagonalMatrix& operator/= (const K& k)
    {
      diag_ /= k;
      return *this;
    }



    //===== linear maps

    //! y = A x
    template<class X, class Y>
    void mv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; ++i)
        y[i] = diag_[i] * x[i];
    }

    //! y = A^T x
    template<class X, class Y>
    void mtv (const X& x, Y& y) const
    {
      mv(x, y);
    }

    //! y += A x
    template<class X, class Y>
    void umv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; ++i)
        y[i] += diag_[i] * x[i];
    }

    //! y += A^T x
    template<class X, class Y>
    void umtv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; ++i)
        y[i] += diag_[i] * x[i];
    }

    //! y += A^H x
    template<class X, class Y>
    void umhv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; i++)
        y[i] += conjugateComplex(diag_[i])*x[i];
    }

    //! y -= A x
    template<class X, class Y>
    void mmv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; ++i)
        y[i] -= diag_[i] * x[i];
    }

    //! y -= A^T x
    template<class X, class Y>
    void mmtv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; ++i)
        y[i] -= diag_[i] * x[i];
    }

    //! y -= A^H x
    template<class X, class Y>
    void mmhv (const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; i++)
        y[i] -= conjugateComplex(diag_[i])*x[i];
    }

    //! y += alpha A x
    template<class X, class Y>
    void usmv (const K& alpha, const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; i++)
        y[i] += alpha * diag_[i] * x[i];
    }

    //! y += alpha A^T x
    template<class X, class Y>
    void usmtv (const K& alpha, const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; i++)
        y[i] += alpha * diag_[i] * x[i];
    }

    //! y += alpha A^H x
    template<class X, class Y>
    void usmhv (const K& alpha, const X& x, Y& y) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (x.N()!=N()) DUNE_THROW(FMatrixError,"index out of range");
      if (y.N()!=M()) DUNE_THROW(FMatrixError,"index out of range");
#endif
      for (size_type i=0; i<n; i++)
        y[i] += alpha * conjugateComplex(diag_[i]) * x[i];
    }

    //===== norms

    //! frobenius norm: sqrt(sum over squared values of entries)
    double frobenius_norm () const
    {
      return diag_.two_norm();
    }

    //! square of frobenius norm, need for block recursion
    double frobenius_norm2 () const
    {
      return diag_.two_norm2();
    }

    //! infinity norm (row sum norm, how to generalize for blocks?)
    double infinity_norm () const
    {
      return diag_.infinity_norm();
    }

    //! simplified infinity norm (uses Manhattan norm for complex values)
    double infinity_norm_real () const
    {
      return diag_.infinity_norm_real();
    }



    //===== solve

    //! Solve system A x = b
    template<class V>
    void solve (V& x, const V& b) const
    {
      for (int i=0; i<n; i++)
        x[i] = b[i]/diag_[i];
    }

    //! Compute inverse
    void invert()
    {
      for (int i=0; i<n; i++)
        diag_[i] = 1/diag_[i];
    }

    //! calculates the determinant of this matrix
    K determinant () const
    {
      K det = diag_[0];
      for (int i=1; i<n; i++)
        det *= diag_[i];
      return det;
    }



    //===== sizes

    //! number of blocks in row direction
    size_type N () const
    {
      return n;
    }

    //! number of blocks in column direction
    size_type M () const
    {
      return n;
    }



    //===== query

    //! return true when (i,j) is in pattern
    bool exists (size_type i, size_type j) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (i<0 || i>=n) DUNE_THROW(FMatrixError,"row index out of range");
      if (j<0 || j>=m) DUNE_THROW(FMatrixError,"column index out of range");
#endif
      return i==j;
    }



    //! Sends the matrix to an output stream
    friend std::ostream& operator<< (std::ostream& s, const DiagonalMatrix<K,n>& a)
    {
      for (size_type i=0; i<n; i++) {
        for (size_type j=0; j<n; j++)
          s << ((i==j) ? a.diag_[i] : 0) << " ";
        s << std::endl;
      }
      return s;
    }

    //! Return reference object as row replacement
    reference operator[](size_type i)
    {
      return reference(const_cast<K*>(&diag_[i]), i);
    }

    //! Return const_reference object as row replacement
    const_reference operator[](size_type i) const
    {
      return const_reference(const_cast<K*>(&diag_[i]), i);
    }

    //! Get const reference to diagonal entry
    const K& diagonal(size_type i) const
    {
      return diag_[i];
    }

    //! Get reference to diagonal entry
    K& diagonal(size_type i)
    {
      return diag_[i];
    }

    //! Get const reference to diagonal vector
    const FieldVector<K,n>& diagonal() const
    {
      return diag_;
    }

    //! Get reference to diagonal vector
    FieldVector<K,n>& diagonal()
    {
      return diag_;
    }

  private:

    // the data, a FieldVector storing the diagonal
    FieldVector<K,n> diag_;
  };


  template<class DiagonalMatrixType>
  class DiagonalMatrixWrapper
  {
    typedef typename DiagonalMatrixType::reference reference;
    typedef typename DiagonalMatrixType::const_reference const_reference;
    typedef typename DiagonalMatrixType::field_type K;
    typedef DiagonalRowVector<K, DiagonalMatrixType::rows> row_type;
    typedef std::size_t size_type;
    typedef DiagonalMatrixWrapper< DiagonalMatrixType> MyType;

    friend class ContainerWrapperIterator<const MyType, reference, reference>;
    friend class ContainerWrapperIterator<const MyType, const_reference, const_reference>;

  public:

    DiagonalMatrixWrapper() :
      mat_(0)
    {}

    DiagonalMatrixWrapper(const DiagonalMatrixType* mat) :
      mat_(const_cast<DiagonalMatrixType*>(mat))
    {}

    size_type realIndex(int i) const
    {
      return i;
    }

    row_type* pointer(int i) const
    {
      row_ = row_type(&(mat_->diagonal(i)), i);
      return &row_;
    }

    bool identical(const DiagonalMatrixWrapper& other) const
    {
      return mat_==other.mat_;
    }

  private:

    mutable DiagonalMatrixType* mat_;
    mutable row_type row_;
  };

  /** \brief
   *
   */
  template< class K, int n >
  class DiagonalRowVectorConst
  {
    template<class DiagonalMatrixType>
    friend class DiagonalMatrixWrapper;
    friend class ContainerWrapperIterator<DiagonalRowVectorConst<K,n>, const K, const K&>;

  public:
    // remember size of vector
    enum { dimension = n };

    // standard constructor and everything is sufficient ...

    //===== type definitions and constants

    //! export the type representing the field
    typedef K field_type;

    //! export the type representing the components
    typedef K block_type;

    //! The type used for the index access and size operation
    typedef std::size_t size_type;

    //! We are at the leaf of the block recursion
    enum {
      //! The number of block levels we contain
      blocklevel = 1
    };

    //! export size
    enum {
      //! The size of this vector.
      size = n
    };

    //! Constructor making uninitialized vector
    DiagonalRowVectorConst() :
      p_(0),
      row_(0)
    {}

    //! Constructor making vector with identical coordinates
    explicit DiagonalRowVectorConst (K* p, int col) :
      p_(p),
      row_(col)
    {}

    //===== access to components

    //! same for read only access
    const K& operator[] (size_type i) const
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (i!=row_)
        DUNE_THROW(FMatrixError,"index is contained in pattern");
#endif
      return *p_;
    }

    // check if row is identical to other row (not only identical values)
    // since this is a proxy class we need to check equality of the stored pointer
    bool identical(const DiagonalRowVectorConst<K,n>& other) const
    {
      return ((p_ == other.p_)and (row_ == other.row_));
    }

    //! ConstIterator class for sequential access
    typedef ContainerWrapperIterator<DiagonalRowVectorConst<K,n>, const K, const K&> ConstIterator;
    //! typedef for stl compliant access
    typedef ConstIterator const_iterator;

    //! begin ConstIterator
    ConstIterator begin () const
    {
      return ConstIterator(*this,0);
    }

    //! end ConstIterator
    ConstIterator end () const
    {
      return ConstIterator(*this,1);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeEnd
    //! instead.
    ConstIterator rbegin() const DUNE_DEPRECATED
    {
      return beforeEnd();
    }

    //! @returns an iterator that is positioned before
    //! the end iterator of the rows. i.e. at the row.
    ConstIterator beforeEnd() const
    {
      return ConstIterator(*this,0);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeBegin
    //! instead.
    ConstIterator rend () const DUNE_DEPRECATED
    {
      return beforeBegin();
    }

    //! @returns an iterator that is positioned before
    //! the first row of the matrix.
    ConstIterator beforeBegin () const
    {
      return ConstIterator(*this,-1);
    }

    //! Binary vector comparison
    bool operator== (const DiagonalRowVectorConst& y) const
    {
      return ((p_==y.p_)and (row_==y.row_));
    }

    //===== sizes

    //! number of blocks in the vector (are of size 1 here)
    size_type N () const
    {
      return n;
    }

    //! dimension of the vector space
    size_type dim () const
    {
      return n;
    }

    //! index of this row in surrounding matrix
    size_type rowIndex() const
    {
      return row_;
    }

    //! the diagonal value
    const K& diagonal() const
    {
      return *p_;
    }

  protected:

    size_type realIndex(int i) const
    {
      return rowIndex();
    }

    K* pointer(size_type i) const
    {
      return const_cast<K*>(p_);
    }

    DiagonalRowVectorConst* operator&()
    {
      return this;
    }

    // the data, very simply a pointer to the diagonal value and the row number
    K* p_;
    size_type row_;
  };

  template< class K, int n >
  class DiagonalRowVector : public DiagonalRowVectorConst<K,n>
  {
    template<class DiagonalMatrixType>
    friend class DiagonalMatrixWrapper;
    friend class ContainerWrapperIterator<DiagonalRowVector<K,n>, K, K&>;

  public:
    // standard constructor and everything is sufficient ...

    //===== type definitions and constants

    //! export the type representing the field
    typedef K field_type;

    //! export the type representing the components
    typedef K block_type;

    //! The type used for the index access and size operation
    typedef std::size_t size_type;

    //! Constructor making uninitialized vector
    DiagonalRowVector() : DiagonalRowVectorConst<K,n>()
    {}

    //! Constructor making vector with identical coordinates
    explicit DiagonalRowVector (K* p, int col) : DiagonalRowVectorConst<K,n>(p, col)
    {}

    //===== assignment from scalar
    //! Assignment operator for scalar
    DiagonalRowVector& operator= (const K& k)
    {
      *p_ = k;
      return *this;
    }

    //===== access to components

    //! random access
    K& operator[] (size_type i)
    {
#ifdef DUNE_FMatrix_WITH_CHECKING
      if (i!=row_)
        DUNE_THROW(FMatrixError,"index is contained in pattern");
#endif
      return *p_;
    }

    //! Iterator class for sequential access
    typedef ContainerWrapperIterator<DiagonalRowVector<K,n>, K, K&> Iterator;
    //! typedef for stl compliant access
    typedef Iterator iterator;

    //! begin iterator
    Iterator begin ()
    {
      return Iterator(*this, 0);
    }

    //! end iterator
    Iterator end ()
    {
      return Iterator(*this, 1);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeEnd
    //! instead.
    Iterator rbegin() DUNE_DEPRECATED
    {
      return beforeEnd();
    }

    //! @returns an iterator that is positioned before
    //! the end iterator of the rows, i.e. at the last row.
    Iterator beforeEnd ()
    {
      return Iterator(*this, 0);
    }

    //! @deprecated This method was renamed to make
    //! it distinct from the STL version which returns
    //! a reverse iterator. Use the new method beforeBegin
    //! instead.
    Iterator rend ()  DUNE_DEPRECATED
    {
      return beforeBegin();
    }

    //! @returns an iterator that is positioned before
    //! the first row of the matrix.
    Iterator beforeBegin ()
    {
      return Iterator(*this, -1);
    }

    //! ConstIterator class for sequential access
    typedef ContainerWrapperIterator<DiagonalRowVectorConst<K,n>, const K, const K&> ConstIterator;
    //! typedef for stl compliant access
    typedef ConstIterator const_iterator;

    using DiagonalRowVectorConst<K,n>::identical;
    using DiagonalRowVectorConst<K,n>::operator[];
    using DiagonalRowVectorConst<K,n>::operator==;
    using DiagonalRowVectorConst<K,n>::begin;
    using DiagonalRowVectorConst<K,n>::end;
    using DiagonalRowVectorConst<K,n>::rbegin;
    using DiagonalRowVectorConst<K,n>::rend;
    using DiagonalRowVectorConst<K,n>::beforeEnd;
    using DiagonalRowVectorConst<K,n>::beforeBegin;
    using DiagonalRowVectorConst<K,n>::N;
    using DiagonalRowVectorConst<K,n>::dim;
    using DiagonalRowVectorConst<K,n>::rowIndex;
    using DiagonalRowVectorConst<K,n>::diagonal;

  protected:

    DiagonalRowVector* operator&()
    {
      return this;
    }

  private:

    using DiagonalRowVectorConst<K,n>::p_;
    using DiagonalRowVectorConst<K,n>::row_;
  };


  // implement type traits
  template<class K, int n>
  struct const_reference< DiagonalRowVector<K,n> >
  {
    typedef DiagonalRowVectorConst<K,n> type;
  };

  template<class K, int n>
  struct const_reference< DiagonalRowVectorConst<K,n> >
  {
    typedef DiagonalRowVectorConst<K,n> type;
  };

  template<class K, int n>
  struct mutable_reference< DiagonalRowVector<K,n> >
  {
    typedef DiagonalRowVector<K,n> type;
  };

  template<class K, int n>
  struct mutable_reference< DiagonalRowVectorConst<K,n> >
  {
    typedef DiagonalRowVector<K,n> type;
  };



  /** \brief Iterator class for sparse vector-like containers
   *
   * This class provides an iterator for sparse vector like containers.
   * It contains a ContainerWrapper that must provide the translation
   * from the position in the underlying container to the index
   * in the sparse container.
   *
   * The ContainerWrapper must be default and copy-constructable.
   * Furthermore it must provide the methods:
   *
   * bool identical(other)      - check if this is identical to other (same container, not only equal)
   * T* pointer(position)       - get pointer to data at position in underlying container
   * size_t realIndex(position) - get index in sparse container for position in underlying container
   *
   * Notice that the iterator stores a ContainerWrapper.
   * This allows to use proxy classes as underlying container
   * and as returned reference type.
   *
   * \tparam CW The container wrapper class
   * \tparam T The contained type
   * \tparam R The reference type returned by dereference
   */
  template<class CW, class T, class R>
  class ContainerWrapperIterator : public BidirectionalIteratorFacade<ContainerWrapperIterator<CW,T,R>,T, R, int>
  {
    typedef typename remove_const<CW>::type NonConstCW;

    friend class ContainerWrapperIterator<CW, typename mutable_reference<T>::type, typename mutable_reference<R>::type>;
    friend class ContainerWrapperIterator<CW, typename const_reference<T>::type, typename const_reference<R>::type>;

    typedef ContainerWrapperIterator<CW, typename mutable_reference<T>::type, typename mutable_reference<R>::type> MyType;
    typedef ContainerWrapperIterator<CW, typename const_reference<T>::type, typename const_reference<R>::type> MyConstType;

  public:

    // Constructors needed by the facade iterators.
    ContainerWrapperIterator() :
      containerWrapper_(),
      position_(0)
    {}

    ContainerWrapperIterator(CW containerWrapper, int position) :
      containerWrapper_(containerWrapper),
      position_(position)
    {}

    template<class OtherContainerWrapperIteratorType>
    ContainerWrapperIterator(OtherContainerWrapperIteratorType& other) :
      containerWrapper_(other.containerWrapper_),
      position_(other.position_)
    {}

    ContainerWrapperIterator(const MyType& other) :
      containerWrapper_(other.containerWrapper_),
      position_(other.position_)
    {}

    ContainerWrapperIterator(const MyConstType& other) :
      containerWrapper_(other.containerWrapper_),
      position_(other.position_)
    {}

    template<class OtherContainerWrapperIteratorType>
    ContainerWrapperIterator& operator=(OtherContainerWrapperIteratorType& other)
    {
      containerWrapper_ = other.containerWrapper_;
      position_ = other.position_;
    }

    // This operator is needed since we can not get the address of the
    // temporary object returned by dereference
    T* operator->() const
    {
      return containerWrapper_.pointer(position_);
    }

    // Methods needed by the forward iterator
    bool equals(const MyType& other) const
    {
      return position_ == other.position_ && containerWrapper_.identical(other.containerWrapper_);
    }

    bool equals(const MyConstType& other) const
    {
      return position_ == other.position_ && containerWrapper_.identical(other.containerWrapper_);
    }

    R dereference() const
    {
      return *containerWrapper_.pointer(position_);
    }

    void increment()
    {
      ++position_;
    }

    // Additional function needed by BidirectionalIterator
    void decrement()
    {
      --position_;
    }

    // Additional function needed by RandomAccessIterator
    R elementAt(int i) const
    {
      return *containerWrapper_.pointer(position_+i);
    }

    void advance(int n)
    {
      position_=position_+n;
    }

    template<class OtherContainerWrapperIteratorType>
    std::ptrdiff_t distanceTo(OtherContainerWrapperIteratorType& other) const
    {
      assert(containerWrapper_.identical(other));
      return other.position_ - position_;
    }

    std::ptrdiff_t index() const
    {
      return containerWrapper_.realIndex(position_);
    }

  private:
    NonConstCW containerWrapper_;
    size_t position_;
  };



  template<class K, int n>
  void istl_assign_to_fmatrix(FieldMatrix<K,n,n>& fm, const DiagonalMatrix<K,n>& s)
  {
    fm = K();
    for(int i=0; i<n; ++i)
      fm[i][i] = s.diagonal()[i];
  }
  /* @} */
} // end namespace
#endif
