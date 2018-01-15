class zooo {

}

aspect asp {
  advice void moop() 
    call( zooo.moop ) 
    call( zooo.poom ) 
  {
    zooo z = this ;
    printf( "advice moop called\n" ) ;
  }
  
}
