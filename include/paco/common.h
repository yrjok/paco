#pragma once

#include <paco/recognizer.h>

#include <string>

namespace paco {

namespace recognizers {

/**
 * Get a recognizer that matches any single lowercase ASCII character.
 */
recognizer lowercase ();

/**
 * Get a recognizer that matches any single uppercase ASCII character.
 */
recognizer uppercase ();

/**
 * Get a recognizer that matches any single lowercase or uppercase ASCII character.
 */
recognizer letter ();

/**
 * Get a recognizer that matches any single arrabic numeral.
 *
 * That is, any character in the set { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
 */
recognizer numeral ();

/**
 * Get a recognizer that matches a single letter, numeral or underscore.
*/
recognizer alfanumeric_char ();

/**
 * Get a recognizer that matches any single whitespace character.
 *
 * That is, either a space, tab, newline or carriage return
*/
recognizer whitespace_char ();

/**
 * Get a recognizer that matches any sequence of lower- and uppercase
 * characters.
*/
recognizer word ();

/**
 * Get a recognizer that matches any integer number existing solely of a
 * sequence of numerals.
*/
recognizer number ();

/**
 * Get a recognizer that matches a sequence of alfanumeric characters
*/
recognizer alfanumeric ();

/**
 * Get a recognizer that matches any amount of whitespace characters.
*/
recognizer whitespace ();

/**
 * Get a recognizer that matches the given character literal.
*/
recognizer literal (char character);

/**
 * Get a recognizer that matches the given string literal.
*/
recognizer literal (std::string const & text);

} // ns recognizers

} // ns paco
