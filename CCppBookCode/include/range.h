// ==========================================================================
//
// Software written by Boguslaw Cyganek (C) to be used with the book:
// INTRODUCTION TO PROGRAMMING WITH C++ FOR ENGINEERS
// Published by Wiley, 2020
//
// The software is supplied as is and for educational purposes
// without any guarantees nor responsibility of its use in any application. 
//
// ==========================================================================




#pragma once


namespace CppBook
{



#include <vector>
#include <cassert>
#include <stdexcept>	// for std::out_of_range




// The range utility. Allows combination of the indexed
// and range based for loops.
// Template type deduction is used.
// Examples:
//
//	for( auto i : range( 123 ) )
//		cout << i << " ";
//	...
//	for( auto i : range( 0.0, 256.0, 16.5 ) ) 
//	{
//		for( auto j : range( -2, 16, 3 ) ) 
//			ostr << j << ", ";
//		ostr << endl << i << endl;
//	}
//
//
template < typename T = size_t >
class range 
{
		const T kFrom, kEnd, kStep;

     public:

		///////////////////////////////////////////////////////////
		// Class constructor 
		///////////////////////////////////////////////////////////
		//
		// INPUT:
		//		from - Starting number of the sequence.
		//		end - Generate numbers up to, but not including this number.
		//		step - Difference between each number in the sequence (must not be 0).
		//
		// REMARKS:
		//		If step is 0 then the std::out_of_range is thrown
		//
		range( const T from, const T end, const T step = 1 ) 
			: kFrom( from ), kEnd( end ), kStep( step ) 
		{
			assert( kStep != 0 );
			if( kStep == 0 )
				throw std::out_of_range( "step param must not be 0" );
		}

		// Default from==0, step==1
		range( const T end ) 
			: kFrom( 0 ), kEnd( end ), kStep( 1 ) 
		{
			assert( kEnd > 0 );
		}

	private:

        class range_iter 
		{
			T fVal {};
			const T kStep {};
		public:
            range_iter( const T v, const T step ) : fVal( v ), kStep( step ) {}
            operator T  () const				{ return fVal; }
            operator const T & ()				{ return fVal; }
            const T operator * () const			{ return fVal; }
            const range_iter & operator ++ ()	{ fVal += kStep; return * this; }
 

			bool operator == ( const range_iter & ri ) const
			{
				return ! operator != ( ri );
			}

			bool operator != ( const range_iter & ri ) const
			{	
				// This is a tricky part - when working with iterators
				// it checks only once for != which must be a hit to stop;
				// However, simple != does not work if increasing 
				//	kStart by N times kSteps skips over kEnd. Therefore this condition:
				return kStep > 0 ? fVal < ri.fVal : fVal > ri.fVal;	
			}      											
		};													

	public:

        const range_iter begin()	{ return range_iter( kFrom, kStep ); }
        const range_iter end()		{ return range_iter( kEnd,	kStep ); }

    public:

		// Conversion to any vector< T >
		operator std::vector< T > ( void ) 
		{
			//std::vector< T > retRange;
			//if( kStep > 0 )
			//	for( T i { kFrom }; i < kEnd; i += kStep ) retRange.push_back( i );
			//else
			//	for( T i { kFrom }; i > kEnd; i += kStep ) retRange.push_back( i );

			//return retRange;	// use RVO here


			auto p = [this]( auto v ){ for( T i {kFrom}; i<kEnd; i+=kStep ) v.push_back(i); return v; };
			auto n = [this]( auto v ){ for( T i {kFrom}; i>kEnd; i+=kStep ) v.push_back(i); return v; };

			return kStep > 0 ? p( std::vector< T >() ) : n( std::vector< T >() );	// use RVO here
		}
};




// ---------------------------------------------------
// Let's introduce some template type deduction guides

range( const size_t ) -> range< size_t >;

range( const unsigned int ) -> range< size_t >;

range( const int ) -> range< int >;

range( char ) -> range< int >;

//range( const char ) -> range< int >;




}	// end of the CppBookCode namespace



