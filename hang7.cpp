template <typename t> class v
{
public:
    friend v<t> (::operator- <>)( const v<t>&, const v<t>& );
};


int
main( void )
{
    v<double> a( 1.0 ), b( 0.0 );
    b = -a;
}
