/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   forwardbutton_png;
    const int            forwardbutton_pngSize = 7397;

    extern const char*   loopbutton_png;
    const int            loopbutton_pngSize = 4178;

    extern const char*   pausebutton_png;
    const int            pausebutton_pngSize = 4248;

    extern const char*   playbutton_png;
    const int            playbutton_pngSize = 7964;

    extern const char*   rewindbutton_png;
    const int            rewindbutton_pngSize = 5772;

    extern const char*   stopbutton_png;
    const int            stopbutton_pngSize = 4064;

    extern const char*   turntable_png;
    const int            turntable_pngSize = 15263;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
