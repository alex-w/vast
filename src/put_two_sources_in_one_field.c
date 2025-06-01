#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h> // for isdigit()

#include "vast_limits.h"

#define ARCSEC_IN_RAD 206264.806247096
// LOL, can't use M_PI in define
// #define ARCSEC_IN_RAD 3600.0 * 180.0 / M_PI

int is_ra_or_dec_string( const char *str ) {
 int i;
 int colon_count= 0;
 int dot_count= 0;
 int digit_count= 0;
 int sign_count= 0;

 for ( i= 0; str[i] != '\0'; i++ ) {
  if ( str[i] == ':' ) {
   colon_count++;
  } else if ( str[i] == '.' ) {
   dot_count++;
  } else if ( str[i] == '+' || str[i] == '-' ) {
   sign_count++;
  } else if ( isdigit( str[i] ) ) {
   digit_count++;
  } else if ( !isspace( str[i] ) ) {
   return 0; // Invalid character found
  }
 }

 // Check if the string matches the expected format
 if ( ( colon_count == 2 && dot_count <= 1 && sign_count <= 1 ) ||  // HH:MM:SS[.sss] or HH:MM:SS[.sss][+-]
      ( colon_count == 0 && dot_count <= 1 && sign_count <= 1 ) ) { // [+-]DDD.ddd or [+-]DDD
  return digit_count > 0;                                           // At least one digit is required
 }

 return 0; // Invalid format
}

int format_hms_or_deg( char *coordinatestring ) {
 unsigned int i;
 for ( i= 0; i < strlen( coordinatestring ); i++ ) {
  if ( coordinatestring[i] == ':' )
   return 1;
 }
 return 0;
}

int compute_angular_distance_and_print_result( char *string_RA1, char *string_Dec1, char *string_RA2, char *string_Dec2, double search_radius_arcsec, double *output_distance_arcsec ) {
 double hh, mm, ss;
 double RA1_deg, DEC1_deg, RA2_deg, DEC2_deg;

 double in, ss2;
 int hh2, mm2;

 double distance;

 double cosine_value;

 // Check the input is not empty
 if ( NULL == string_RA1 ) {
  fprintf( stderr, "ERROR string_RA1 is NULL\n" );
  return 1;
 }
 if ( NULL == string_Dec1 ) {
  fprintf( stderr, "ERROR string_Dec1 is NULL\n" );
  return 1;
 }

 if ( NULL == string_RA2 ) {
  fprintf( stderr, "ERROR string_RA2 is NULL\n" );
  return 1;
 }
 if ( NULL == string_Dec2 ) {
  fprintf( stderr, "ERROR string_Dec2 is NULL\n" );
  return 1;
 }

 if ( strlen( string_RA1 ) < 1 ) {
  fprintf( stderr, "ERROR string_RA1 is too short\n" );
  return 1;
 }
 if ( strlen( string_Dec1 ) < 1 ) {
  fprintf( stderr, "ERROR string_Dec1 is too short\n" );
  return 1;
 }

 if ( strlen( string_RA2 ) < 1 ) {
  fprintf( stderr, "ERROR string_RA2 is too short\n" );
  return 1;
 }
 if ( strlen( string_Dec2 ) < 1 ) {
  fprintf( stderr, "ERROR string_Dec2 is too short\n" );
  return 1;
 }

 // Parse the first set of coordinates
 if ( format_hms_or_deg( string_RA1 ) ) {
  // Format HH:MM:SS.SS
  sscanf( string_RA1, "%lf:%lf:%lf", &hh, &mm, &ss );
  // Check the values
  if ( hh < 0 || hh > 23 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' hh understood as '%lf'\n", string_RA1, hh );
   return 1;
  }
  if ( mm < 0 || mm > 59 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' mm understood as '%lf'\n", string_RA1, mm );
   return 1;
  }
  if ( ss < 0 || ss > 60 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' ss understood as '%lf'\n", string_RA1, ss );
   return 1;
  }
  //
  hh+= mm / 60 + ss / 3600;
  hh*= 15;
  RA1_deg= hh;
 } else {
  // Format DD.DDDD
  RA1_deg= atof( string_RA1 );
 }

 if ( format_hms_or_deg( string_Dec1 ) ) {
  // Format DD:MM:SS.S
  sscanf( string_Dec1, "%lf:%lf:%lf", &hh, &mm, &ss );
  // Check the values
  if ( hh < -89 || hh > 89 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' hh understood as '%lf'\n", string_Dec1, hh );
   return 1;
  }
  if ( mm < 0 || mm > 59 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' mm understood as '%lf'\n", string_Dec1, mm );
   return 1;
  }
  if ( ss < 0 || ss > 60 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' ss understood as '%lf'\n", string_Dec1, ss );
   return 1;
  }
  //
  if ( hh >= 0 && string_Dec1[0] != '-' )
   hh+= mm / 60 + ss / 3600;
  else
   hh-= mm / 60 + ss / 3600;
  DEC1_deg= hh;
 } else {
  // Format DD.DDDD
  DEC1_deg= atof( string_Dec1 );
 }

 // Check the resulting values
 if ( RA1_deg < 0.0 || RA1_deg > 360.0 ) {
  fprintf( stderr, "ERROR parsing input coordinates: '%s' understood as '%lf'\n", string_RA1, RA1_deg );
  return 1;
 }
 if ( DEC1_deg < -90.0 || DEC1_deg > 90.0 ) {
  fprintf( stderr, "ERROR parsing input coordinates: '%s' understood as '%lf'\n", string_Dec1, DEC1_deg );
  return 1;
 }

 // Parse the second set of coordinates
 if ( format_hms_or_deg( string_RA2 ) ) {
  // Format HH:MM:SS.SS
  sscanf( string_RA2, "%lf:%lf:%lf", &hh, &mm, &ss );
  // Check the values
  if ( hh < 0 || hh > 23 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' hh understood as '%lf'\n", string_RA2, hh );
   return 1;
  }
  if ( mm < 0 || mm > 59 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' mm understood as '%lf'\n", string_RA2, mm );
   return 1;
  }
  if ( ss < 0 || ss > 60 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' ss understood as '%lf'\n", string_RA2, ss );
   return 1;
  }
  //
  hh+= mm / 60 + ss / 3600;
  hh*= 15;
  RA2_deg= hh;
 } else {
  // Format DD.DDDD
  RA2_deg= atof( string_RA2 );
 }

 if ( format_hms_or_deg( string_Dec2 ) ) {
  // Format DD:MM:SS.S
  sscanf( string_Dec2, "%lf:%lf:%lf", &hh, &mm, &ss );
  // Check the values
  if ( hh < -89 || hh > 89 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' hh understood as '%lf'\n", string_Dec2, hh );
   return 1;
  }
  if ( mm < 0 || mm > 59 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' mm understood as '%lf'\n", string_Dec2, mm );
   return 1;
  }
  if ( ss < 0 || ss > 60 ) {
   fprintf( stderr, "ERROR parsing input coordinates: '%s' ss understood as '%lf'\n", string_Dec2, ss );
   return 1;
  }
  //
  if ( hh >= 0 && string_Dec2[0] != '-' )
   hh+= mm / 60 + ss / 3600;
  else
   hh-= mm / 60 + ss / 3600;
  DEC2_deg= hh;
 } else {
  // Format DD.DDDD
  DEC2_deg= atof( string_Dec2 );
 }

 // Check the resulting values
 if ( RA2_deg < 0.0 || RA2_deg > 360.0 ) {
  fprintf( stderr, "ERROR parsing input coordinates: '%s' understood as '%lf'\n", string_RA2, RA2_deg );
  return 1;
 }
 if ( DEC2_deg < -90.0 || DEC2_deg > 90.0 ) {
  fprintf( stderr, "ERROR parsing input coordinates: '%s' understood as '%lf'\n", string_Dec2, DEC2_deg );
  return 1;
 }

 if ( 0.0 == search_radius_arcsec ) {
  // fprintf( stderr, "%lf %lf  %lf %lf\n", RA1_deg, DEC1_deg, RA2_deg, DEC2_deg );
  fprintf( stderr, "%.10lf %.10lf  %.10lf %.10lf\n", RA1_deg, DEC1_deg, RA2_deg, DEC2_deg );
 }

 if ( MAX( RA1_deg, RA2_deg ) > 180 && MIN( RA1_deg, RA2_deg ) < 180 ) {
  if ( RA1_deg > 180 )
   RA1_deg-= 360;
  if ( RA2_deg > 180 )
   RA1_deg-= 360;
 }
 in= RA1_deg + RA2_deg;
 in= in / 2.0;

 if ( in < 0.0 ) {
  in+= 360;
 }

 in= in / 15.0;
 hh2= (int)in;
 mm2= (int)( ( in - hh2 ) * 60 );
 ss2= ( ( in - hh2 ) * 60 - mm2 ) * 60;
 // fprintf( stderr, "DEBUG00: %lf %d %d %lf\n",in, hh2, mm2, ss2);
 if ( fabs( ss2 - 60.0 ) < 0.01 ) {
  mm2+= 1;
  ss2= 0.0;
  // fprintf( stderr, "DEBUG01: %lf %d %d %lf\n",in, hh2, mm2, ss2);
 }
 if ( mm2 == 60 ) {
  hh2+= 1;
  mm2= 0.0;
  // fprintf( stderr, "DEBUG02: %lf %d %d %lf\n",in, hh2, mm2, ss2);
 }
 if ( 0.0 == search_radius_arcsec ) {
  // fprintf( stderr, "DEBUG: %.10lf %d %d %lf\n", in, hh2, mm2, ss2 );
  fprintf( stdout, "Average position  %02d:%02d:%05.2lf ", hh2, mm2, ss2 );
 }

 in= ( DEC1_deg + DEC2_deg ) / 2.0;
 hh2= (int)in;
 mm2= (int)( ( in - hh2 ) * 60 );
 ss2= ( ( in - hh2 ) * 60 - mm2 ) * 60;
 if ( in < 0.0 ) {
  mm2*= -1;
  ss2*= -1;
 }
 if ( fabs( ss2 - 60.0 ) < 0.01 ) {
  mm2+= 1;
  ss2= 0.0;
 }
 if ( mm2 == 60 ) {
  hh2+= 1;
  mm2= 0.0;
 }
 // if ( ( hh2 == 0 && in < 0 ) || ( hh2 == 0 && mm2 == 0 && in < 0 ) ) {
 if ( in < 0 ) {
  if ( 0.0 == search_radius_arcsec )
   fprintf( stdout, "-%02d:%02d:%04.1lf\n", abs( hh2 ), mm2, fabs( ss2 ) );
 } else {
  if ( 0.0 == search_radius_arcsec )
   fprintf( stdout, "+%02d:%02d:%04.1lf\n", hh2, mm2, ss2 );
 }

 RA1_deg*= 3600.0 / ARCSEC_IN_RAD;
 RA2_deg*= 3600.0 / ARCSEC_IN_RAD;
 DEC1_deg*= 3600.0 / ARCSEC_IN_RAD;
 DEC2_deg*= 3600.0 / ARCSEC_IN_RAD;

 // we may get a nan if the distance is exactly zero, so let's catch this situation early
 if ( RA1_deg == RA2_deg && DEC1_deg == DEC2_deg ) {
  distance= 0.0;
 } else {
  //
  cosine_value= cos( DEC1_deg ) * cos( DEC2_deg ) * cos( MAX( RA1_deg, RA2_deg ) - MIN( RA1_deg, RA2_deg ) ) + sin( DEC1_deg ) * sin( DEC2_deg );
  // don't trust acos() to properly handle the cosine_value=+/-1 cases, so we chack the boundary values ourselves
  if ( cosine_value >= 1.0 ) {
   distance= 0.0;
  } else {
   if ( cosine_value <= -1.0 ) {
    distance= M_PI;
   } else {
    // distance= acos(cos(DEC1_deg) * cos(DEC2_deg) * cos(MAX(RA1_deg, RA2_deg) - MIN(RA1_deg, RA2_deg)) + sin(DEC1_deg) * sin(DEC2_deg));
    distance= acos( cosine_value );
   }
  }
 }
 // check if the trick worked
 if ( 0 != isnan( distance ) ) {
  fprintf( stderr, "ERROR in %s distance is 'nan'\n", "compute_angular_distance_and_print_result()" );
  return 1;
 }

 in= distance * ARCSEC_IN_RAD / 3600;
 hh2= (int)in;
 mm2= (int)( ( in - hh2 ) * 60 );
 ss2= ( ( in - hh2 ) * 60 - mm2 ) * 60;
 if ( fabs( ss2 - 60.0 ) < 0.01 ) {
  mm2+= 1;
  ss2= 0.0;
 }
 if ( mm2 == 60 ) {
  hh2+= 1;
  mm2= 0.0;
 }

 if ( 0.0 == search_radius_arcsec ) {
  fprintf( stdout, "Angular distance  %02d:%02d:%05.2lf = ", hh2, mm2, ss2 );
  fprintf( stdout, "%lf degrees\n", distance * ARCSEC_IN_RAD / 3600 );
 } else {
  // We are in the source list matching mode
  if ( distance * ARCSEC_IN_RAD < search_radius_arcsec ) {
   ( *output_distance_arcsec )= distance * ARCSEC_IN_RAD;
   return 0;
  } else {
   return 1;
  }
 }

 return 0;
}

int main( int argc, char **argv ) {
 FILE *filelist_input_positions;
 char str1[512];
 char str2[512];
 char str_comment[512];
 int sscanf_return_value;
 char input_buffer[4096];
 double search_radius_arcsec;
 unsigned int i;
 int argument_counter; // I'm comparing it to argc and argc is signed int type
 int string_looks_ok;
 int string_contains_number;
 int string_dot_or_semicolon;

 double output_distance_arcsec= 99.99;

 if ( argc < 5 ) {
  fprintf( stderr, "Usage of %s:\n%s RA1 DEC1 RA2 DEC2\nor\n%s RA1 DEC1 radeclist.txt search_radius_arcsec\n", argv[0], argv[0], argv[0] );
  //
  fprintf( stderr, "Actual command line was: " );
  for ( argument_counter= 0; argument_counter < argc; argument_counter++ ) {
   fprintf( stderr, "%s ", argv[argument_counter] );
  }
  fprintf( stderr, "\n" );
  //
  return 1;
 }

 // Arguments 1 and 2 should be RA and Dec
 if ( 0 == is_ra_or_dec_string( argv[1] ) ) {
  fprintf( stderr, "An ERROR occured in %s the first command line option (%s) does not look like an RA string.\n", argv[0], argv[1] );
  return 1;
 }
 if ( 0 == is_ra_or_dec_string( argv[2] ) ) {
  fprintf( stderr, "An ERROR occured in %s the second command line option (%s) does not look like a Dec string.\n", argv[0], argv[2] );
  return 1;
 }
 // Argument 3 may be RA string or a file name, we'll check that in a moment
 // Argument 4 is either Dec or search radius in arcsec
 if ( 0 == is_ra_or_dec_string( argv[4] ) ) {
  fprintf( stderr, "An ERROR occured in %s the fourth command line option (%s) does not look like a Dec string or search radius in arcsec.\n", argv[0], argv[4] );
  return 1;
 }

 // check if the third argment is a file name
 filelist_input_positions= fopen( argv[3], "r" );
 if ( NULL == filelist_input_positions ) {
  //
  if ( 0 == is_ra_or_dec_string( argv[3] ) ) {
   fprintf( stderr, "An ERROR occured in %s the third command line option (%s) is not a valid file name and does not look like an RA string.\n", argv[0], argv[3] );
   return 1;
  }
  //
  // it is not - compare just one pair of positions and exit
  if ( 0 != compute_angular_distance_and_print_result( argv[1], argv[2], argv[3], argv[4], 0.0, &output_distance_arcsec ) ) {
   fprintf( stderr, "An ERROR occured in %s while running compute_angular_distance_and_print_result()\n", argv[0] );
   return 1;
  } else {
   return 0;
  }
 }
 // if we are still here, the third argment is a file name
 // the file is expected to contain a list of positions
 search_radius_arcsec= atof( argv[4] );
 if ( search_radius_arcsec <= 0.0 || search_radius_arcsec > 3600.0 ) {
  fprintf( stderr, "ERROR in %s -- invalid search radius in arcsec: %s\n", argv[0], argv[4] );
  return 1;
 }
 while ( NULL != fgets( input_buffer, 4096, filelist_input_positions ) ) {
  // check that the string contains white space and then something
  input_buffer[4096 - 1]= '\0';
  string_looks_ok= 0;
  for ( i= 1; i < strlen( input_buffer ) - 2; i++ ) {
   if ( input_buffer[i] == ' ' && input_buffer[i + 1] != ' ' ) {
    string_looks_ok= 1;
    break;
   }
  }
  if ( 0 == string_looks_ok ) {
   continue;
  }
  // check that the string contains a number
  string_contains_number= 0;
  for ( i= 0; i < strlen( input_buffer ) - 1; i++ ) {
   if ( 0 != isdigit( input_buffer[i] ) ) {
    string_contains_number= 1;
   }
  }
  if ( 0 == string_contains_number ) {
   continue;
  }
  // check that the string contains dot or semicolon
  string_dot_or_semicolon= 0;
  for ( i= 0; i < strlen( input_buffer ) - 1; i++ ) {
   if ( input_buffer[i] == '.' || input_buffer[i] == ':' ) {
    string_dot_or_semicolon= 1;
   }
  }
  if ( 0 == string_dot_or_semicolon ) {
   continue;
  }

  // OK, this may be a "RA DEC" or "RA DEC COMMENTS" type string
  // sscanf_return_value= sscanf( input_buffer, "%s %s %[^\n]", str1, str2, str_comment );
  sscanf_return_value= sscanf( input_buffer, "%511s %511s %511[^\n]", str1, str2, str_comment );

  if ( sscanf_return_value < 2 ) {
   str1[0]= '\0';
   str2[0]= '\0';
   str_comment[0]= '\0';
   continue;
  }
  str1[512 - 1]= '\0';
  str2[512 - 1]= '\0';
  if ( sscanf_return_value == 2 ) {
   str_comment[0]= '\0';
  } else {
   str_comment[512 - 1]= '\0';
  }
  if ( 0 == compute_angular_distance_and_print_result( argv[1], argv[2], str1, str2, search_radius_arcsec, &output_distance_arcsec ) ) {
   fprintf( stdout, "FOUND  %4.1lf\"  %s\n", output_distance_arcsec, str_comment );
   break;
  }
 }
 fclose( filelist_input_positions );

 return 0;
}
