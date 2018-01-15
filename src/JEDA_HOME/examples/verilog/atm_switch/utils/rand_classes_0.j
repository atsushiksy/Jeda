/* 
   
   Copyright (C) 2001 Juniper Networks Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/
abstract class random_distribution {
  abstract int gen_random() ;
  abstract void get_mean(var int mean_num_result,
			  var int mean_denom_result);
}

class uniform_distribution extends random_distribution {
  int min_value, max_value, multiplier;
  random_mt rnd ;
  
  void new(int a_min_value, int a_max_value, int a_multiplier=1 ) {
    if (a_min_value > a_max_value) {
      error(
        "min_value(%0d) > max_value(%0d)\n", a_min_value, a_max_value
      );
    }
    min_value = a_min_value;
    max_value = a_max_value;
    multiplier = a_multiplier;
    rnd = new() ;
  }
  
  int gen_random() {
    int ret ;
    if (min_value == max_value) {
      ret = min_value * multiplier;
    } else {
      ret = 
        (min_value + (rnd.random() % (max_value - min_value + 1))) * 
          multiplier;
    }
    return ret ;
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = (min_value + max_value) * multiplier;
    mean_denom_result = 2;
  }
}

class constant_distribution extends random_distribution {  
  int const ;
  
  new( int value ) {
    const = value ;
  }
  
  int gen_random() {
    return  const ;
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = const;
    mean_denom_result = 1;
  }

}

class constant_loop_distribution extends random_distribution {  
    int loop_num[];
    int value[];
    int num_value ;
    int current = 0 ;
    int lcurrent = 0 ;
    int mean ;

  void new(int a_loop_nump[], int a_value[], int a_num_value) {
    int i ;
    int n = 0 ;
    num_value = a_num_value ;
    for( i = 0 ; i < num_value ; i++ ) {
      loop_num[i] = a_loop_nump[i] ;
      value[i] = a_value[i] ;
      mean += a_value[i] * loop_num[i] ;
      n += loop_num[i] ;
    }
    mean /= n ;
  }

  int gen_random() {
    int ret ;
    
    if( lcurrent == loop_num[current] ) {
      current++ ;
      current %= num_value ;
      lcurrent = 0 ; 
    }
    else lcurrent++ ;
    ret = value[current] ;
    return ret ;
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = mean;
    mean_denom_result = 1;
  }

}

/* Geometric distribution with p value = p_numerator/p_denominator */
class geometric_distribution extends random_distribution {
  int p_numerator, p_denominator ;
  geom_random rnd ;

  void new(int a_p_numerator, int a_p_denominator) {
    p_numerator = a_p_numerator ;
    p_denominator = a_p_denominator ;
    rnd = new(p_numerator, p_denominator) ;
  }

  int gen_random() {
    return rnd.random();
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = p_denominator/p_numerator;
    mean_denom_result = p_denominator;
  }
}

/* Expornential distribution with mean */
class expornential_distribution extends random_distribution {
  int p_numerator, p_denominator ;
  exp_random rnd ;
  int mean ;

  void new(int a_mean) {
    rnd = new(a_mean) ;
    mean = a_mean ;
  }

  int gen_random() {
    return rnd.random();
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = mean ;
    mean_denom_result = 1 ;
  }
}

class pdf_distribution extends random_distribution {
  pdf_random rnd ;
  int sum ;
  int weights ;
  
  void new() {
    rnd = new() ;
    sum = 0 ;
  }

  void set( int weight, int value ) {
    rnd.set( weight, value ) ;
    sum += (weight * value) ;
    weights += weight ;
  }
  
  int gen_random() {
    return rnd.random();
  }

  void get_mean(var int mean_num_result, var int mean_denom_result)
  {
    mean_num_result = sum/weights ;
    mean_denom_result = 1 ;
  }
}
