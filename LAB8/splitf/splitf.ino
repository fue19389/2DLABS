void setup (){}
// Define 
String str = "This is my string"; 

// Length (with one extra character for the null terminator)
int str_len = str.length() + 1; 

// Prepare the character array (the buffer) 
char char_array[str_len];

// Copy it over 
str.toCharArray(char_array, str_len);

void loop(){
  
}
