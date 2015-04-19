// Copyright (C) 2012 Andrei Alexandrescu.

#ifndef NONSTD_EXPECTED_ALEXANDRESCU_H_INCLUDED
#define NONSTD_EXPECTED_ALEXANDRESCU_H_INCLUDED

#include <algorithm>            // std::move()
#include <exception>            // std::exception_ptr

template<class T>
class Expected
{
    union
    {
        T ham;
        std::exception_ptr spam;
    };

    bool gotHam;

    Expected() {} //used internally

public:
    ~Expected()
    {
        if ( gotHam )
            ham.~T();
        else
            spam.~exception_ptr();
    }

    Expected( const T & rhs ) : ham( rhs ), gotHam( true ) {}

    Expected( T && rhs ) : ham( std::move( rhs ) ), gotHam( true ) {}

    Expected( const Expected & rhs )
        : gotHam( rhs.gotHam )
    {
        if( gotHam )
            new( &ham ) T ( rhs.ham );
        else
            new( &spam ) std::exception_ptr( rhs.spam );
    }

    Expected( Expected && rhs )
        : gotHam( rhs.gotHam )
    {
        if( gotHam )
            new( &ham ) T ( std::move( rhs.ham ) );
        else
            new( &spam ) std::exception_ptr( std::move( rhs.spam ) );

    }

    void swap( Expected & rhs )
    {
        if( gotHam )
        {
            if( rhs.gotHam )
            {
                using std::swap;
                swap( ham, rhs.ham );
            }
            else
            {
                auto t = std::move( rhs.spam );
                new( &rhs.ham ) T( std::move( ham ) );
                new( &spam ) std::exception_ptr( t );
                std::swap( gotHam, rhs.gotHam );
            }
        }
        else
        {
            if( rhs.gotHam )
            {
                rhs.swap( *this );
            }
            else
            {
                spam.swap( rhs.spam );
                std::swap( gotHam, rhs.gotHam );
            }
        }
    }

    template<class E>
    static Expected<T> fromException( const E & exception )
    {
        if( typeid( exception ) != typeid( E ) )
        {
            throw std::invalid_argument( "slicing detected" );
        }
        return fromException( std::make_exception_ptr( exception ) );
    }

    static Expected<T> fromException( std::exception_ptr p )
    {
        Expected<T> result;
        result.gotHam = false;

        new( &result.spam ) std::exception_ptr( std::move( p ) );

        return result;
    }

    static Expected<T> fromException()
    {
        return fromException( std::current_exception() );
    }

    // Access:

    bool valid() const
    {
        return gotHam;
    }

    T & get()
    {
        if( !gotHam )
            std::rethrow_exception( spam );
        return ham;

    }

    const T & get() const
    {
        if( !gotHam )
            std::rethrow_exception( spam );
        return ham;
    }

    // Probing for Type:

    template<class E>
    bool hasException() const
    {
        try
        {
            if( !gotHam )
                std::rethrow_exception( spam );
        }
        catch
            ( const E & object )
        {
            return true;
        }
        catch
            ( ... )
        {
        }
        return false;

    }

    // Icing:

    template<class F>
    static Expected fromCode( F fun )
    {
        try
        {
            return Expected( fun() );
        }
        catch
            ( ... )
        {
            return fromException();
        }
    }

    // auto r = Expected<string>::fromCode( [] { ... } );
};

#endif // NONSTD_EXPECTED_ALEXANDRESCU_H_INCLUDED
