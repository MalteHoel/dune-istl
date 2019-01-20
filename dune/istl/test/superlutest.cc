// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include <config.h>

#include <complex>

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/timer.hh>

#include <dune/istl/bvector.hh>
#include <dune/istl/operators.hh>
#include <dune/istl/superlu.hh>

#include "laplacian.hh"

#if HAVE_SUPERLU
template<typename Matrix, typename Vector>
void runSuperLU(std::size_t N)
{
  using Operator = Dune::MatrixAdapter<Matrix,Vector,Vector>;

  Matrix mat;
  Operator fop(mat);
  Vector b(N*N), x(N*N), b1(N/2), x1(N/2);

  setupLaplacian(mat,N);
  b=1;
  b1=1;
  x=0;
  x1=0;

  Dune::Timer watch;

  watch.reset();

  Dune::SuperLU<Matrix> solver(mat, true);

  Dune::InverseOperatorResult res;

  Dune::SuperLU<Matrix> solver1;

  solver.setVerbosity(true);
  solver.apply(x,b, res);

  std::set<std::size_t> mrs;
  for(std::size_t s=0; s < N/2; ++s)
    mrs.insert(s);

  solver1.setSubMatrix(mat,mrs);

  solver1.apply(x1,b1, res);
  solver1.apply(reinterpret_cast<typename Matrix::field_type*>(&x1[0]),
                reinterpret_cast<typename Matrix::field_type*>(&b1[0]));
}
#endif

int main(int argc, char** argv)
try
{
#if HAVE_SUPERLU
  std::size_t N=100;

  if(argc>1)
    N = atoi(argv[1]);

  // ------------------------------------------------------------------------------
  std::cout<<"testing for N="<<N<<" BCRSMatrix<FieldMatrix<T,1,1> >"<<std::endl;
#if HAVE_SLU_SDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<float,1,1> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<float,1> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_DDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<double,1,1> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<double,1> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_CDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<std::complex<float>,1,1> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<std::complex<float>,1> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_ZDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<std::complex<double>,1,1> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<std::complex<double>,1> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif

  // ------------------------------------------------------------------------------
  std::cout<<"testing for N="<<N<<" BCRSMatrix<FieldMatrix<T,2,2> >"<<std::endl;
#if HAVE_SLU_SDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<float,2,2> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<float,2> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_DDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<double,2,2> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<double,2> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_CDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<std::complex<float>,2,2> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<std::complex<float>,2> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif
#if HAVE_SLU_ZDEFS_H
  {
    using Matrix = Dune::BCRSMatrix<Dune::FieldMatrix<std::complex<double>,2,2> >;
    using Vector = Dune::BlockVector<Dune::FieldVector<std::complex<double>,2> >;
    runSuperLU<Matrix,Vector>(N);
  }
#endif

  return 0;

#else // HAVE_SUPERLU
  std::cerr << "You need SuperLU to run this test." << std::endl;
  return 77;
#endif // HAVE_SUPERLU
}
catch (std::exception &e)
{
  std::cout << "ERROR: " << e.what() << std::endl;
  return 1;
}
catch (...)
{
  std::cerr << "Dune reported an unknown error." << std::endl;
  exit(1);
}
