/* test list.sort() function */
void main() 
{
  string str_list<>;
  int i, n ;
  
  str_list.push( "bifosotre" ) ;
  str_list.push( "eodfikepre" ) ;
  str_list.push( "v efgehher" ) ;
  str_list.push( "fwgegeherh" ) ;
  str_list.push( "ferfeher" ) ;
  str_list.push( "bsifosotreer" ) ;
  str_list.push( "baifosoeterre" ) ;
  str_list.push( "bvifosotreeteye" ) ;
  str_list.push( "bfifosoetertre" ) ;
  str_list.push( "bcifosoertre" ) ;
  str_list.push( "bdirwsfosotre" ) ;
  str_list.push( "beiforeysotre" ) ;
  str_list.push( "bnifosteotre" ) ;
  str_list.push( "abifosotre" ) ;
  str_list.push( "bbifosotre" ) ;
  str_list.push( "cbifosotre" ) ;
  str_list.push( "dbifosotre" ) ;
  str_list.push( "badirwsfosotre" ) ;
  str_list.push( "bceiforeysotre" ) ;
  str_list.push( "bcnifosteotre" ) ;
  str_list.push( "abifosotrea" ) ;
  str_list.push( "abbifosotre" ) ;
  str_list.push( "acbifosotre" ) ;
  str_list.push( "adbifosotre" ) ;

  printf( "str_list.num_element() = %0d\n", str_list.num_element() ) ;
  
  printf( "calling str_list.sort()\n" ) ;
   
  str_list.sort() ;
   
  n = str_list.num_element() ;
  printf( "str_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "str_list<%d> = %s\n", i, str_list.pop() ) ;
  }
  
  
}
