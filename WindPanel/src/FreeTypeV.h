#ifndef FREETYPEV_H
#define FREETYPEV_H

//MSVC will spit out all sorts of useless warnings if
//you create vectors of strings, this pragma gets rid of them.
//#pragma warning(disable: 4786)

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
//
//OpenGL Headers 
//#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>
// #include "CSU_INI/ini_struct.hpp"


struct RectOgl
{
        double left, right, bottom, top;

};

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace freetypeV
{

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
    using std::vector;

//Ditto for string.
    using std::string;

//This holds all of the information related to any
//freetype font that we want to create.  
    struct font_data
    {
            float h;			///< Holds the height of the font.
            GLuint * textures;     ///< Holds the texture id's
            GLuint list_base;     ///< Holds the first display list id

            //The object in which Freetype holds information on a given
            //font is called a "face".
            FT_Face face;
            GLuint * dist;

//            int x1;
//            int x2;

            //The init function will create a font of
            //of the height h from the file fname.
            void
            init (const char * fname,
                  unsigned int h);
//                  int x1,
//                  int x2);

            //Free all the resources assosiated with the font.
            void
            clean ();
    };

//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text. 
    float
    print (const font_data &ft_font,
           float x,
           float y,
           RectOgl *rectOrtho,
           const wchar_t *fmt,
           ...);

/*
 float
 print (const font_data &ft_font,
 float x,
 float y,
 float z,
 const wchar_t *fmt,
 ...);
 */

}

#endif
