/*
 A quick and simple opengl font library that uses GNU freetype2, written
 and distributed as part of a tutorial for nehe.gamedev.net.
 Sven Olsen, 2003
 */

//Include our header file.
#include "FreeTypeV.h"
#include <exception>
#include <stdexcept>

#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <iostream>
#include "tbb/atomic.h"
#include <vector>

extern log4cxx::LoggerPtr loggerMain;


namespace freetypeV
{

///This function gets the first power of 2 >= the
///int that we pass it.
    inline int
    next_p2 (int a)
    {
        int rval = 2;
        while (rval < a)
            rval <<= 1;
        return rval;
    }

///Create a display list coresponding to the give character.
    void
    make_dlist (FT_Face face,
                unsigned short ch,
                GLuint list_base,
                GLuint * tex_base,
                GLuint * dist)
    {

        //The first thing we do is get FreeType to render our character
        //into a bitmap.  This actually requires a couple of FreeType commands:

        //Load the Glyph for our character.
        if (FT_Load_Glyph (face, FT_Get_Char_Index (face, ch), FT_LOAD_DEFAULT))
            throw std::runtime_error ("FT_Load_Glyph failed");

        //Move the face's glyph into a Glyph object.
        FT_Glyph glyph;
        if (FT_Get_Glyph (face->glyph, &glyph))
            throw std::runtime_error ("FT_Get_Glyph failed");

        //Convert the glyph to a bitmap.
        FT_Glyph_To_Bitmap (&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

        //This reference will make accessing the bitmap easier
        FT_Bitmap& bitmap = bitmap_glyph->bitmap;

        //Use our helper function to get the widths of
        //the bitmap data that we will need in order to create
        //our texture.
        int width = next_p2 (bitmap.width);
        int height = next_p2 (bitmap.rows);

        //Allocate memory for the texture data.
        //GLubyte* expanded_data = new GLubyte[2 * width * height];
        GLubyte expanded_data[2 * width * height];

// Here We Fill In The Data For The Expanded Bitmap.
        // Notice That We Are Using A Two Channel Bitmap (One
        // Channel For Luminosity And One For Alpha Values).
        //
        // We Make All The Luminosity Values White, And Use The Freeytpe Generated Bitmap
        // To Set Up The Alpha Values. Given The Blend Function That We're Going To Use,
        // This Will Make OpenGL Render The Font Properly.
        //
        // We Use The ?: Operator To Say That The Alpha Value Which We Use
        // Will Be 0 If We Are In The Padding Zone, And Whatever
        // Is The FreeType Bitmap Otherwise.
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++)
            {
                expanded_data[2 * (i + j * width)] = 255;
                expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
            }

        //Now we just setup some texture paramaters.
        glBindTexture (GL_TEXTURE_2D, tex_base[ch]);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Here we actually create the texture itself, notice
        //that we are using GL_LUMINANCE_ALPHA to indicate that
        //we are using 2 channel data.
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, &expanded_data);

        //With the texture created, we don't need to expanded data anymore
        //delete[] expanded_data;

        //So now we can create the display list
        glNewList (list_base + ch, GL_COMPILE);

        glBindTexture (GL_TEXTURE_2D, tex_base[ch]);

        glPushMatrix ();

        //first we need to move over a little so that
        //the character has the right amount of space
        //between it and the one before it.
        glTranslatef (bitmap_glyph->left, 0, 0);

        //Now we move down a little in the case that the
        //bitmap extends past the bottom of the line 
        //(this is only true for characters like 'g' or 'y'.
        if (bitmap.rows > bitmap_glyph->top)
            glTranslatef (0, -std::abs ((int) bitmap_glyph->top - (int) bitmap.rows), 0);
        else if (bitmap.rows < bitmap_glyph->top)
            glTranslatef (0, std::abs ((int) bitmap_glyph->top - (int) bitmap.rows), 0);

//        glTranslatef (0, bitmap_glyph->top -bitmap.rows, 0);

        //Now we need to account for the fact that many of
        //our textures are filled with empty padding space.
        //We figure what portion of the texture is used by 
        //the actual character and store that information in 
        //the x and y variables, then when we draw the
        //quad, we will only reference the parts of the texture
        //that we contain the character itself.
        float x = (float) bitmap.width / (float) width, y = (float) bitmap.rows / (float) height;

        //Here we draw the texturemaped quads.
        //The bitmap that we got from FreeType was not 
        //oriented quite like we would like it to be,
        //so we need to link the texture to the quad
        //so that the result will be properly aligned.
        glBegin (GL_QUADS);
        glTexCoord2d (0, 0);
        glVertex2f (0, bitmap.rows);
        glTexCoord2d (0, y);
        glVertex2f (0, 0);
        glTexCoord2d (x, y);
        glVertex2f (bitmap.width, 0);
        glTexCoord2d (x, 0);
        glVertex2f (bitmap.width, bitmap.rows);
        glEnd ();
        glPopMatrix ();
        glTranslatef (face->glyph->advance.x >> 6, 0, 0);

        dist[ch] = (face->glyph->advance.x >> 6);

        //increment the raster position as if we were a bitmap font.
        //(only needed if you want to calculate text length)
//        glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

        //Finnish the display list
        glEndList ();
        FT_Done_Glyph (glyph);
    }

    ///Create a display list coresponding to the give character. // AFR
    void
    displayList (FT_Face face,
                 unsigned char ch,
                 GLuint list_base,
                 GLuint * tex_base,
                 GLuint * dist)
    {

        //The first thing we do is get FreeType to render our character
        //into a bitmap.  This actually requires a couple of FreeType commands:

        //Load the Glyph for our character.
        if (FT_Load_Glyph (face, FT_Get_Char_Index (face, ch), FT_LOAD_DEFAULT))
            throw std::runtime_error ("FT_Load_Glyph failed");

        //Move the face's glyph into a Glyph object.
        FT_Glyph glyph;
        if (FT_Get_Glyph (face->glyph, &glyph))
            throw std::runtime_error ("FT_Get_Glyph failed");

//        dist[ch] = face->glyph->advance.x >> 6;

        //Convert the glyph to a bitmap.
        FT_Glyph_To_Bitmap (&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

        //This reference will make accessing the bitmap easier
        FT_Bitmap& bitmap = bitmap_glyph->bitmap;

        //Use our helper function to get the widths of
        //the bitmap data that we will need in order to create
        //our texture.
        int width = next_p2 (bitmap.width);
        int height = next_p2 (bitmap.rows);

        //Allocate memory for the texture data.
        //GLubyte* expanded_data = new GLubyte[2 * width * height];
        GLubyte expanded_data[2 * width * height];

        // Here We Fill In The Data For The Expanded Bitmap.
        // Notice That We Are Using A Two Channel Bitmap (One
        // Channel For Luminosity And One For Alpha Values).
        //
        // We Make All The Luminosity Values White, And Use The Freeytpe Generated Bitmap
        // To Set Up The Alpha Values. Given The Blend Function That We're Going To Use,
        // This Will Make OpenGL Render The Font Properly.
        //
        // We Use The ?: Operator To Say That The Alpha Value Which We Use
        // Will Be 0 If We Are In The Padding Zone, And Whatever
        // Is The FreeType Bitmap Otherwise.
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++)
            {
                expanded_data[2 * (i + j * width)] = 255;
                expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
            }

        //Now we just setup some texture paramaters.
        glBindTexture (GL_TEXTURE_2D, tex_base[ch]);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Here we actually create the texture itself, notice
        //that we are using GL_LUMINANCE_ALPHA to indicate that
        //we are using 2 channel data.
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, &expanded_data);

        //With the texture created, we don't need to expanded data anymore
        //delete[] expanded_data;

        //So now we can create the display list
        glNewList (list_base + ch, GL_COMPILE);

        glBindTexture (GL_TEXTURE_2D, tex_base[ch]);

        glPushMatrix ();

        //first we need to move over a little so that
        //the character has the right amount of space
        //between it and the one before it.
        glTranslatef (bitmap_glyph->left, 0.0, 0.0);

        //Now we move down a little in the case that the
        //bitmap extends past the bottom of the line
        //(this is only true for characters like 'g' or 'y'.
        //glTranslatef (0.0, (float) bitmap_glyph->top - bitmap.rows, 0.0); // TODO: revisar caracteres con letras mas bajas de la horizontal

        if (bitmap.rows > bitmap_glyph->top)
            glTranslatef (0, -std::abs ((int) bitmap_glyph->top - (int) bitmap.rows), 0);
        else if (bitmap.rows < bitmap_glyph->top)
            glTranslatef (0, std::abs ((int) bitmap_glyph->top - (int) bitmap.rows), 0);

        //        glTranslatef (0, bitmap_glyph->top -bitmap.rows, 0);

        //Now we need to account for the fact that many of
        //our textures are filled with empty padding space.
        //We figure what portion of the texture is used by
        //the actual character and store that information in
        //the x and y variables, then when we draw the
        //quad, we will only reference the parts of the texture
        //that we contain the character itself.
        float x = (float) bitmap.width / (float) width;
        float y = (float) bitmap.rows / (float) height;

        //Here we draw the texturemaped quads.
        //The bitmap that we got from FreeType was not
        //oriented quite like we would like it to be,
        //so we need to link the texture to the quad
        //so that the result will be properly aligned.
        glBegin (GL_QUADS);
        glTexCoord2d (0, 0);
        glVertex2f (0.0, (float) bitmap.rows);
        glTexCoord2d (0, y);
        glVertex2f (0.0, 0.0);
        glTexCoord2d (x, y);
        glVertex2f ((float) bitmap.width, 0.0);
        glTexCoord2d (x, 0);
        glVertex2f ((float) bitmap.width, (float) bitmap.rows);
        glEnd ();
        glPopMatrix ();
        glTranslatef ((float) (face->glyph->advance.x >> 6), 0.0, 0.0);

        dist[ch] = face->glyph->advance.x >> 6;

        //increment the raster position as if we were a bitmap font.
        //(only needed if you want to calculate text length)
        //        glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

        //Finnish the display list
        glEndList ();
        FT_Done_Glyph (glyph);
    }

    void
    font_data::init (const char * fname,
                     unsigned int h)
//                     ,int x1,
//                     int x2)
    {
//        this->x1 = x1;
//        this->x2 = x2;
        //Allocate some memory to store the texture ids.
        textures = new GLuint[512];
        dist = new GLuint[512];

        this->h = h;

        //Create and initilize a freetype font library.
        FT_Library library;
        if (FT_Init_FreeType (&library))
            throw std::runtime_error ("FT_Init_FreeType failed");

        //This is where we load in the font information from the file.
        //Of all the places where the code might die, this is the most likely,
        //as FT_New_Face will die if the font file does not exist or is somehow broken.
        if (FT_New_Face (library, fname, 0, &face))
        {
//            throw std::runtime_error ("FT_New_Face failed (there is probably a problem with your font file)");
            std::cout << "FT_New_Face failed (there is probably a problem with your font file) " << std::endl;
        }
        //For some twisted reason, Freetype measures font size
        //in terms of 1/64ths of pixels.  Thus, to make a font
        //h pixels high, we need to request a size of h*64.
        //(h << 6 is just a prettier way of writting h*64)
        FT_Set_Char_Size (face, ((long) h) << 6, ((long) h) << 6, 96, 96);

        //Here we ask opengl to allocate resources for
        //all the textures and displays lists which we
        //are about to create.  
        list_base = glGenLists (256);
        glGenTextures (256, textures);

        //This is where we actually create each of the fonts display lists.
        for (unsigned short i = 0; i < 256; i++)
            make_dlist (face, i, list_base, textures, dist);

        //We don't need the face information now that the display
        //lists have been created, so we free the assosiated resources.
        FT_Done_Face (face);

        //Ditto for the library.
        FT_Done_FreeType (library);
    }
//    void
//    font_data::init (const char * fname,  // AFR
//                     unsigned int h,
//                     int x1,
//                     int x2)
//    {
//        this->x1 = x1;
//        this->x2 = x2;
//        //Allocate some memory to store the texture ids.
//        textures = new GLuint[256];
//        dist = new GLuint[256];
//
//        this->h = h;
//
//        //Create and initilize a freetype font library.
//        FT_Library library;
//        if (FT_Init_FreeType (&library))
//            throw std::runtime_error ("FT_Init_FreeType failed");
//
//        //This is where we load in the font information from the file.
//        //Of all the places where the code might die, this is the most likely,
//        //as FT_New_Face will die if the font file does not exist or is somehow broken.
//        if (FT_New_Face (library, fname, 0, &face))
//            throw std::runtime_error ("FT_New_Face failed (there is probably a problem with your font file)");
//
////        cont Resource *data = FAH::Viewer::resource0.find(fname);
////        if (data!)
////            throw std::runtime_error ("Failed to find font: " << fname);
////
////        int err;
////        if ((err = FT_New_Memory_Face(library, (uint8_t *)data->getData(), data->getLength(), 0, &face)))
////            throw std::runtime_error ("FT_New_Memory_Face failed to read: " << fname << ": " << err);
//
//        //For some twisted reason, Freetype measures font size
//        //in terms of 1/64ths of pixels.  Thus, to make a font
//        //h pixels high, we need to request a size of h*64.
//        //(h << 6 is just a prettier way of writting h*64)
//        FT_Set_Char_Size (face, h << 6, h << 6, 96, 96);
//
//        //Here we ask opengl to allocate resources for
//        //all the textures and displays lists which we
//        //are about to create.
//        list_base = glGenLists (128);
//        glGenTextures (128, textures);
//
//        //This is where we actually create each of the fonts display lists.
//        for (int i = 0; i < 128; i++)
//            displayList (face, i, list_base, textures, dist);
//
//        //We don't need the face information now that the display
//        //lists have been created, so we free the assosiated resources.
//        FT_Done_Face (face);
//
//        //Ditto for the library.
//        FT_Done_FreeType (library);
//    }

    void
    font_data::clean ()
    {
//        FT_Done_Face (face);

        glDeleteLists (list_base, 256);
        glDeleteTextures (256, textures);
        delete[] textures;
        delete[] dist;
    }

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
    inline void
    pushScreenCoordinateMatrix ()
    {
        glPushAttrib (GL_TRANSFORM_BIT);
        GLint viewport[4];
        glGetIntegerv (GL_VIEWPORT, viewport);

        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();
    }

/// Pops the projection matrix without changing the current
/// MatrixMode.
    inline void
    pop_projection_matrix ()
    {
        glPushAttrib (GL_TRANSFORM_BIT);
        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();
        glPopAttrib ();

    }

    float
    print (const font_data &ft_font,
           float x,
           float y,
           RectOgl *rectOrtho,
           const wchar_t *fmt,
           ...)
    {
        float len = 0;
        try
        {

            // We want a coordinate system where things coresponding to window pixels.
             pushScreenCoordinateMatrix ();



           /// gluOrtho2D (ft_font.x1, ft_font.x2, viewport[1], viewport[3]);
 /*           std::cout << "1 viewport : " << viewport[0] <<"  "  << viewport[1] <<"  "<<  viewport[2] <<"  "<< viewport[3] <<"  "<< std::endl;
            std::cout << "1 viewport : " << viewport[0] <<"  "  << viewport[1] <<"  "<<  viewport[2] <<"  "<< viewport[3] <<"  "<< std::endl;
*///            GLdouble left, GLdouble right, GLdouble bottom, GLdouble top
            glOrtho (rectOrtho->left, rectOrtho->right,  rectOrtho->bottom, rectOrtho->top, -1., 1.);
//             std::cout << " gluOrtho2D  "<<   (double)( rectOrtho->right -rectOrtho->left)/(double)viewport[2] <<"  " << (double)( rectOrtho->top -rectOrtho->bottom)/(double)viewport[3] << "    left: "  << rectOrtho->left <<"  right: "<< rectOrtho->right <<"  bottom: "<<rectOrtho->bottom <<"  top: "<<  rectOrtho->top << std::endl;




   /*         double xortho= (double)viewport[0] * (double)( rectOrtho->right -rectOrtho->left)/(double)viewport[2];
            double yortho= (double)viewport[1] * (double)( rectOrtho->top -rectOrtho->bottom)/(double)viewport[3];
*/
            glPopAttrib ();

            GLuint font = ft_font.list_base;
            float h = ft_font.h / .63f;		//We make the height about 1.5* that of
            wchar_t text[256];        // Holds Our String

            va_list ap;								// Pointer To List Of Arguments

            if (fmt == NULL)								// If There's No Text
                *text = 0;											// Do Nothing

            else
            {
                va_start(ap, fmt);				// Parses The String For Variables
                vswprintf ((wchar_t *) text, 256, (const wchar_t *) fmt, ap);				// And Converts Symbols To Actual Numbers
                va_end(ap);							// Results Are Stored In Text
            }

            //Here is some code to split the text that we have been
            //given into a set of lines.  
            //This could be made much neater by using
            //a regular expression library such as the one avliable from
            //boost.org (I've only done it out by hand to avoid complicating
            //this tutorial with unnecessary library dependencies).
            const wchar_t *start_line = text;
            std::vector<std::string> lines;
            const wchar_t *c;
            for (c = text; *c; c++)
            {

                if (*c == '\n')
                {
                    std::string line;
                    for (const wchar_t *n = start_line; n < c; n++)
                    {
                        line.append (1, *n);

                    }
                    lines.push_back (line);
                    start_line = c + 1;
                }
                len = len + ft_font.dist[*c];

            }
            if (start_line)
            {
                std::string line;
                for (const wchar_t *n = start_line; n < c; n++)
                    line.append (1, *n);
                lines.push_back (line);
            }

            glPushAttrib (GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
            glMatrixMode (GL_MODELVIEW);
            glDisable (GL_LIGHTING);
            glEnable (GL_TEXTURE_2D);
            glDisable (GL_DEPTH_TEST);
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            {
//            GLint viewport[4];
//            glGetIntegerv (GL_VIEWPORT, viewport);
//            std::cout << "2 viewport : " << viewport[0] <<"  "  << viewport[1] <<"  "<<  viewport[2] <<"  "<< viewport[3] <<"  "<< std::endl;
//
//            }

            glListBase (font);



            //This is where the text display actually happens.
            //For each line of text we reset the modelview matrix
            //so that the line's text will start in the correct position.
            //Notice that we need to reset the matrix, rather than just translating
            //down by h. This is because when each character is
            //draw it modifies the current matrix so that the next character
            //will be drawn immediatly after it.  
//            glPushMatrix ();
//            glTranslatef (- xortho, -yortho , 0);

            float modelview_matrix[16];
                 glGetFloatv (GL_MODELVIEW_MATRIX, modelview_matrix);

 /*                std::cout<<modelview_matrix[0]<<", "<<modelview_matrix[1]<<", "<<modelview_matrix[2]<<", "<<std::cout<<modelview_matrix[3]<< ","<<len<<std::endl;
                 std::cout<<modelview_matrix[4]<<", "<<modelview_matrix[5]<<", "<<modelview_matrix[6]<<", "<<std::cout<<modelview_matrix[7]<< ","<<len<<std::endl;
                 std::cout<<modelview_matrix[8]<<", "<<modelview_matrix[9]<<", "<<modelview_matrix[10]<<", "<<std::cout<<modelview_matrix[11]<< ","<<len<<std::endl;
                 std::cout<<modelview_matrix[9]<<", "<<modelview_matrix[10]<<", "<<modelview_matrix[11]<<", "<<std::cout<<modelview_matrix[3]<< ","<<len<<std::endl;
*/
            for (int i = 0; i < lines.size (); i++)
            {

                glPushMatrix ();
                glLoadIdentity ();
//                glTranslatef (-viewport[0] , -viewport[1], 0);
                glTranslatef (x , y  - h * i, 0);
                glMultMatrixf (modelview_matrix);

                //  The commented out raster position stuff can be useful if you need to
                //  know the length of the text that you are creating.
                //  If you decide to use it make sure to also uncomment the glBitmap command
                //  in make_dlist().
                glRasterPos2f (0, 0);
                glCallLists (lines[i].length (), GL_UNSIGNED_BYTE, lines[i].c_str ());
//                	float rpos[4];
//                	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);

//                 std::cout<<"len1: "<<len<<std::endl;
                //                	if(x-rpos[0]<0)
//                	 len=abs(x-rpos[0]);
//                  std::cout<<"len2: "<<len<<std::endl;
//                	else
//                	    len= lines[i].length() * (ft_font.h);
//                	std::cout<<"len: "<<len<<std::endl;

//                  std::cout<<"len: "<<len<< " rpos[0] "<< rpos[0]<<std::endl;
//                  std::cout<<"ft_font.x1-x: "<<(ft_font.x1-x )<<" x "<< x << "  c "<< c-text  << " length " <<lines[i].length() << " ft_font.h "<<ft_font.h <<std::endl;

                glPopMatrix ();

            }
//            glPopMatrix ();

            glPopAttrib ();

            pop_projection_matrix ();


        }
        catch (std::exception* e)
        {
            LOG4CXX_ERROR(loggerMain, std::string("freetypeV ").append( e->what()));

        }

        return len;
    }

/*   float
 print (const font_data &ft_font,
 float x,
 float y,
 float z,
 const wchar_t *fmt,
 ...)
 {
 float len = 0;
 try
 {

 // We want a coordinate system where things coresponding to window pixels.
 // pushScreenCoordinateMatrix ();

 glPushAttrib (GL_TRANSFORM_BIT);
 GLint viewport[4];
 glGetIntegerv (GL_VIEWPORT, viewport);
 glMatrixMode (GL_PROJECTION);
 glPushMatrix ();
 glLoadIdentity ();
 gluOrtho2D (ft_font.x1, ft_font.x2, viewport[1], viewport[3]);
 glPopAttrib ();

 GLuint font = ft_font.list_base;
 float h = ft_font.h / .63f;      //We make the height about 1.5* that of
 wchar_t text[256];        // Holds Our String

 va_list ap;          // Pointer To List Of Arguments

 if (fmt == NULL)         // If There's No Text
 *text = 0;           // Do Nothing

 else
 {
 va_start(ap, fmt);         // Parses The String For Variables
 vswprintf ((wchar_t *) text, 256, (const wchar_t *) fmt, ap);     // And Converts Symbols To Actual Numbers
 va_end(ap);           // Results Are Stored In Text
 }

 //Here is some code to split the text that we have been
 //given into a set of lines.
 //This could be made much neater by using
 //a regular expression library such as the one avliable from
 //boost.org (I've only done it out by hand to avoid complicating
 //this tutorial with unnecessary library dependencies).
 const wchar_t *start_line = text;
 vector<string> lines;
 const wchar_t *c;
 for (c = text; *c; c++)
 {

 if (*c == '\n')
 {
 string line;
 for (const wchar_t *n = start_line; n < c; n++)
 {
 line.append (1, *n);

 }
 lines.push_back (line);
 start_line = c + 1;
 }
 len = len + ft_font.dist[*c];

 }
 if (start_line)
 {
 string line;
 for (const wchar_t *n = start_line; n < c; n++)
 line.append (1, *n);
 lines.push_back (line);
 }

 glPushAttrib (GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
 glMatrixMode (GL_MODELVIEW);
 glDisable (GL_LIGHTING);
 glEnable (GL_TEXTURE_2D);
 //        glDisable (GL_DEPTH_TEST);
 glEnable (GL_DEPTH_TEST);

 glEnable (GL_BLEND);
 glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 glListBase (font);

 float modelview_matrix[16];
 glGetFloatv (GL_MODELVIEW_MATRIX, modelview_matrix);

 //This is where the text display actually happens.
 //For each line of text we reset the modelview matrix
 //so that the line's text will start in the correct position.
 //Notice that we need to reset the matrix, rather than just translating
 //down by h. This is because when each character is
 //draw it modifies the current matrix so that the next character
 //will be drawn immediatly after it.
 for (int i = 0; i < lines.size (); i++)
 {

 glPushMatrix ();
 glLoadIdentity ();
 glTranslatef (x, y - h * i, z);
 glMultMatrixf (modelview_matrix);

 //  The commented out raster position stuff can be useful if you need to
 //  know the length of the text that you are creating.
 //  If you decide to use it make sure to also uncomment the glBitmap command
 //  in make_dlist().
 glRasterPos2f (0, 0);
 glCallLists (lines[i].length (), GL_UNSIGNED_BYTE, lines[i].c_str ());
 //                 float rpos[4];
 //                 glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);

 //                 std::cout<<"len1: "<<len<<std::endl;
 //                 if(x-rpos[0]<0)
 //                  len=abs(x-rpos[0]);
 //                  std::cout<<"len2: "<<len<<std::endl;
 //                 else
 //                     len= lines[i].length() * (ft_font.h);
 //                 std::cout<<"len: "<<len<<std::endl;

 //                  std::cout<<"len: "<<len<< " rpos[0] "<< rpos[0]<<std::endl;
 //                  std::cout<<"ft_font.x1-x: "<<(ft_font.x1-x )<<" x "<< x << "  c "<< c-text  << " length " <<lines[i].length() << " ft_font.h "<<ft_font.h <<std::endl;

 glPopMatrix ();

 }

 glPopAttrib ();

 pop_projection_matrix ();
 }
 catch (std::exception e)
 {
 LOG4CXX_ERROR(loggerMain, "freetypeV "<< e.what());

 }

 return len;
 }
 */
}

