# CodeV
Code V related files and troubleshooting

## Contents:

\dll folder : Sample files for User Defined Gradient Index. See the next section for more details

defaults.seq : Initializes the environment for practical purposes. If you did not install Code V in the default location, change paths accordingly. This must be located in CVUSER, and it will be run automatically at the start of each Code V session.

axial_profile: Plots the axial profile of a beam between the before the last surface and the image plane, and returns the FWHM dimensions of the beam's hotspot. This macro was designed for Bessel beam generation, see the file's header for more details.


## Applying a custom user-defined gradient index (UDG)

Code V documentation specifies many ways to create a custom gradient index (GRIN). However, none of them, as they are presented in the documentation as well as the associated sample source files provided through the installation, worked for me. Writing a macro with Macro-PLUS programming with the sample file usergrn.seq did not work, the same applies to generate a .dll through the sample .c source code. I also did not want to use Fortran to construct a .dll since it is quite easy to encounter issues if you don't compile on the appropriate Fortran compiler version. Here I present a robust approach that should work for everyone considering Code V is only available for 64-bit Windows 10 operating system at the time I write this guide (CODEV115 version). Basically, the procedure is to create a .dll file from a .c code (should also work for .cpp) through a modified version of a sample file without the use of the Code V Makefile. Instead, I use MinGW to compile. Here are the steps to follow:

### 1. Install utility tools

Download and install MinGW: https://www.ics.uci.edu/~pattis/common/handouts/mingweclipse/mingw.html. For me, weirdly, step 10 did not work. A simple workaround is to open the command prompt (type CMD on the Windows entry) and copy the following (correct the path if you did not install MinGW directly in C:\ ):

  set PATH=C:\MinGW\bin;%Path%
  
You don't need to add the following to your Path if you only use MinGW for Code V UDG, but if you plan on further applications of this tool the following can become useful:
  
  set PATH=C:\MinGW\msys\1.0\bin;%Path%
  
### 2. Code your UDG profile

For this, I strongly suggest to start from one of my codes, either dll\grnaxicon.c or dll\grnaxicon_v2.c (modified versions of the sample files), to write your own code since I did a few modifications on the structure of the files to make it work. You just have to change the instructions in the body of the function to get your desired profile, and give the function name you want. The description of the needed parameters to generate an UDG are done in the heading of the files, or you can always read the Code V documentation for those. Make sure your function name match your filename. The gradient index profile comes from the following article https://www.osapublishing.org/ol/abstract.cfm?uri=ol-46-6-1333, dll\grnaxicon.c is the exact mathematical formulation of the profile whereas dll\grnaxicon_v2.c is the Taylor series expansion of the same profile. I encourage you to check the repo of the article's author (https://github.com/mirequem/CODEV) where she managed to implement the GRIN Taylor expansion profile through an intuitive Code V macro.

### 3. Generate your .dll

You should now have a file filename.c with a function of the same name as your UDG routine. To create a .dll with MinGW, we first need to compile the source code into a shareable object (.o extension), and then compile this object into a .dll. Open your command prompt (CMD), navigate to the location of your source code and type the following by substituting your own filename (if you did not install MinGW in the default location, change the path accordingly):

  a. C:\MinGW\bin\mingw32-gcc -c filename.c
  
  b. C:\MinGw\bin\mingw32-gcc -shared filename.o -o filename.dll
  
A .dll of the same name as your filename should now have been generated.

### 4. Define your custom GRIN material
 
Open Code V and open the .lens file of the system where you wish to apply your UDG profile (or directly create a new optical system to test your routine). You first need to define a custom glass. Go to Lens -> Add Private Catalog Glass -> Gradient Index... . Give to the GRIN the name you want (8 characters maximum) and choose User Defined as the type. The default step size should be enough for most applications, but by giving a smaller value you will have more accurate results, which also means longer computation time. Enter a line for each wavelength of which you have index data. The Index entry is your base index (brind variable in the source code). Then you enter the coefficients needed for the calculations of your UDG profile. Make sure to respect the order of your source code when you type them. Then click OK.

Alternatively, you can enter your GRIN material through the command line like this:

PRV

PWL wavelength1 wavelength2 ...

'glass_name' brind1 brind2 ...

UDG

UDG C1 value_wavelength1 value_wavelength2 ...

UDG C2 value_wavelength1 value_wavelength2 ...

...

END

Where PWL are the wavelengths for which you have data, in nm. The 'glass_name' is a name of your choice (8 characters maximum), make sure to put it in quotes, and the brindi is the base refractive index of wavelength i. UDG C1 refers to the first coefficient in your UDG profile, and then you list the values for each wavelength. Repeat this for all the coefficients needed to compute your UDG (maximum of 150 coefficients).

Note that you have to redefine your GRIN material, i.e. redo this step, for each .lens file where you want to use your UDG subroutine. For practical purpose, you could write a macro to do this for you if you plan on using your glass in many files.

### 5. Apply your GRIN material and UDG routine

Select the surface where you wish to apply your UDG, and for the glass entry, type your 'glass_name'. Don't forget the quotes, they are needed no matter which way your defined your GRIN material. The quotes tell Code V to look into your private catalogue to apply the glass. Then, right click on the glass, select Surface Properties and go to Gradient Index. In User Defined GRIN Name, browse to your .dll. Your UDG profile should now have been applied without any issue. If you need to modify your UDG routine, you have to do it in the source file and recompile it following step 3.
