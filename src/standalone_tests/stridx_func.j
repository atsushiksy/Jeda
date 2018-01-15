/* test list.sort() function */
void main() 
{
  int str_index[*] ;
  string str_list<>;
  int i, n ;
  
  i = 0 ;
  
  str_index["bifosotre"] = i++ ;
  str_index["eodfikepre"] = i++ ;
  str_index["v efgehher"] = i++ ;
  str_index["fwgegeherh"] = i++ ;
  str_index["ferfeher"] = i++ ;
  str_index["bsifosotreer"] = i++ ;
  str_index["baifosoeterre"] = i++ ;
  str_index["bvifosotreeteye"] = i++ ;
  str_index["bfifosoetertre"] = i++ ;
  str_index["bcifosoertre"] = i++ ;
  str_index["bdirwsfosotre"] = i++ ;
  str_index["beiforeysotre"] = i++ ;
  str_index["bnifosteotre"] = i++ ;
  str_index["abifosotre"] = i++ ;
  str_index["bbifosotre"] = i++ ;
  str_index["cbifosotre"] = i++ ;
  str_index["dbifosotre"] = i++ ;
  str_index["badirwsfosotre"] = i++ ;
  str_index["bceiforeysotre"] = i++ ;
  str_index["bcnifosteotre"] = i++ ;
  str_index["abifosotrea"] = i++ ;
  str_index["abbifosotre"] = i++ ;
  str_index["acbifosotre"] = i++ ;
  str_index["adbifosotre"] = i++ ;

  printf( "str_index.num_element() = %0d\n", str_index.num_element() ) ;
  
  str_list = str_index.index_list() ;
  
  printf( "str_list.num_element() = %0d\n", str_list.num_element() ) ;
  printf( "calling str_list.sort()\n" ) ;
   
  str_list.sort() ;
   
  n = str_list.num_element() ;
  printf( "str_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "str_list<%d> = %s\n", i, str_list.pop() ) ;
  }
  
  str_index.delete_index( "bceiforeysotre" ) ;
  str_index.delete_index( "bfifosoetertre" ) ;
  str_index.delete_index( "eodfikepre" ) ;

  printf( "str_index.num_element() = %0d\n", str_index.num_element() ) ;

  str_list = str_index.index_list() ;
  
  printf( "str_list.num_element() = %0d\n", str_list.num_element() ) ;
  printf( "calling str_list.sort()\n" ) ;
   
  str_list.sort() ;
   
  n = str_list.num_element() ;
  printf( "str_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "str_list<%d> = %s\n", i, str_list.pop() ) ;
  }
  
}
