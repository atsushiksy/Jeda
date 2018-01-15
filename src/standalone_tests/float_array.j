main() {
  int i;
  float floatarray[3];

  for(i=0; i<3; i++) {
    floatarray[i] = 0.1234 * (i*10);
    //floatarray[i] = 00.1234 * (i*10);
    floatarray[i] = .1234 * (i*10);
  }


}
