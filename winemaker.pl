#!/usr/bin/perl -w
use utf8;
use strict;

# Copyright 2000-2004 Fran�ois Gouget for CodeWeavers
# Copyright 2004 Dimitrie O. Paun
# Copyright 2009-2012 Andr� Hentschel
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
#

my $version="0.8.4";

use Cwd;
use File::Basename;
use File::Copy;



#####
#
# Options
#
#####

# The following constants define what we do with the case of filenames

##
# Never rename a file to lowercase
my $OPT_LOWER_NONE=0;

##
# Rename all files to lowercase
my $OPT_LOWER_ALL=1;

##
# Rename only files that are all uppercase to lowercase
my $OPT_LOWER_UPPERCASE=2;


# The following constants define whether to ask questions or not

##
# No (synonym of never)
my $OPT_ASK_NO=0;

##
# Yes (always)
my $OPT_ASK_YES=1;

##
# Skip the questions till the end of this scope
my $OPT_ASK_SKIP=-1;


# The following constants define the architecture

##
# Default Architecture will be chosen
my $OPT_ARCH_DEFAULT=0;

##
# 32-Bit Target
my $OPT_ARCH_32=32;

##
# 64-Bit Target
my $OPT_ARCH_64=64;


# General options

##
# This is the directory in which winemaker will operate.
my $opt_work_dir;

##
# This is the file in which winemaker will operate if a project file is specified.
my $opt_work_file;

##
# Make a backup of the files
my $opt_backup;

##
# Defines which files to rename
my $opt_lower;

##
# If we don't find the file referenced by an include, lower it
my $opt_lower_include;

##
# If true then winemaker should not attempt to fix the source.  This is
# useful if the source is known to be already in a suitable form and is
# readonly
my $opt_no_source_fix;

# Options for the 'Source' method

##
# Specifies that we have only one target so that all sources relate
# to this target. By default this variable is left undefined which
# means winemaker should try to find out by itself what the targets
# are. If not undefined then this contains the name of the default
# target (without the extension).
my $opt_single_target;

##
# If '$opt_single_target' has been specified then this is the type of
# that target. Otherwise it specifies whether the default target type
# is guiexe or cuiexe.
my $opt_target_type;

##
# Contains the default set of flags to be used when creating a new target.
my $opt_flags;

##
# Contains 32 for 32-Bit-Targets and 64 for 64-Bit-Targets
my $opt_arch;

##
# If true then winemaker should ask questions to the user as it goes
# along.
my $opt_is_interactive;
my $opt_ask_project_options;
my $opt_ask_target_options;

##
# If false then winemaker should not generate the makefiles.
my $opt_no_generated_files;

##
# Specifies not to print the banner if set.
my $opt_no_banner;



#####
#
# Target modelization
#
#####

# The description of a target is stored in an array. The constants
# below identify what is stored at each index of the array.

##
# This is the name of the target.
my $T_NAME=0;

##
# Defines the type of target we want to build. See the TT_xxx
# constants below
my $T_TYPE=1;

##
# This is a bitfield containing flags refining the way the target
# should be handled. See the TF_xxx constants below
my $T_FLAGS=2;

##
# This is a reference to an array containing the list of the
# resp. C, C++, RC, other (.h, .hxx, etc.) source files.
my $T_SOURCES_C=3;
my $T_SOURCES_CXX=4;
my $T_SOURCES_RC=5;
my $T_SOURCES_MISC=6;

##
# This is a reference to an array containing the list of
# C compiler options
my $T_CEXTRA=7;

##
# This is a reference to an array containing the list of
# C++ compiler options
my $T_CXXEXTRA=8;

##
# This is a reference to an array containing the list of
# RC compiler options
my $T_RCEXTRA=9;

##
# This is a reference to an array containing the list of macro
# definitions
my $T_DEFINES=10;

##
# This is a reference to an array containing the list of directory
# names that constitute the include path
my $T_INCLUDE_PATH=11;

##
# Flags for the linker
my $T_LDFLAGS=12;

##
# Flags for the archiver
my $T_ARFLAGS=13;

##
# Same as T_INCLUDE_PATH but for the dll search path
my $T_DLL_PATH=14;

##
# The list of Windows dlls to import
my $T_DLLS=15;

##
# Same as T_INCLUDE_PATH but for the library search path
my $T_LIBRARY_PATH=16;

##
# The list of Unix libraries to link with
my $T_LIBRARIES=17;


# The following constants define the recognized types of target

##
# This is not a real target. This type of target is used to collect
# the sources that don't seem to belong to any other target. Thus no
# real target is generated for them, we just put the sources of the
# fake target in the global source list.
my $TT_SETTINGS=0;

##
# For executables in the windows subsystem
my $TT_GUIEXE=1;

##
# For executables in the console subsystem
my $TT_CUIEXE=2;

##
# For dynamically linked libraries
my $TT_DLL=3;

##
# For static libraries
my $TT_LIB=4;


# The following constants further refine how the target should be handled

##
# This target is an MFC-based target
my $TF_MFC=4;

##
# User has specified --nomfc option for this target or globally
my $TF_NOMFC=8;

##
# --nodlls option: Do not use standard DLL set
my $TF_NODLLS=16;

##
# --nomsvcrt option: Do not link with msvcrt
my $TF_NOMSVCRT=32;

##
# This target has a def file (only use it with TT_DLL)
my $TF_HASDEF=64;

##
# This target has C++ files named *.cxx (instead of *.cpp)
my $TF_HASCXX=128;

##
# Initialize a target:
# - set the target type to TT_SETTINGS, i.e. no real target will
#   be generated.
sub target_init($)
{
  my $target=$_[0];

  @$target[$T_TYPE]=$TT_SETTINGS;
  @$target[$T_FLAGS]=$opt_flags;
  @$target[$T_SOURCES_C]=[];
  @$target[$T_SOURCES_CXX]=[];
  @$target[$T_SOURCES_RC]=[];
  @$target[$T_SOURCES_MISC]=[];
  @$target[$T_CEXTRA]=[];
  @$target[$T_CXXEXTRA]=[];
  @$target[$T_RCEXTRA]=[];
  @$target[$T_DEFINES]=[];
  @$target[$T_INCLUDE_PATH]=[];
  @$target[$T_LDFLAGS]=[];
  @$target[$T_ARFLAGS]=[];
  @$target[$T_DLL_PATH]=[];
  @$target[$T_DLLS]=[];
  @$target[$T_LIBRARY_PATH]=[];
  @$target[$T_LIBRARIES]=[];
}



#####
#
# Project modelization
#
#####

# First we have the notion of project. A project is described by an
# array (since we don't have structs in perl). The constants below
# identify what is stored at each index of the array.

##
# This is the path in which this project is located. In other
# words, this is the path to the Makefile.
my $P_PATH=0;

##
# This index contains a reference to an array containing the project-wide
# settings. The structure of that arrray is actually identical to that of
# a regular target since it can also contain extra sources.
my $P_SETTINGS=1;

##
# This index contains a reference to an array of targets for this
# project. Each target describes how an executable or library is to
# be built. For each target this description takes the same form as
# that of the project: an array. So this entry is an array of arrays.
my $P_TARGETS=2;

##
# Initialize a project:
# - set the project's path
# - initialize the target list
# - create a default target (will be removed later if unnecessary)
sub project_init($$$)
{
  my ($project, $path, $global_settings)=@_;

  my $project_settings=[];
  target_init($project_settings);
  @$project_settings[$T_DEFINES]=[@{@$global_settings[$T_DEFINES]}];
  @$project_settings[$T_INCLUDE_PATH]=[@{@$global_settings[$T_INCLUDE_PATH]}];
  @$project_settings[$T_DLL_PATH]=[@{@$global_settings[$T_DLL_PATH]}];
  @$project_settings[$T_DLLS]=[@{@$global_settings[$T_DLLS]}];
  @$project_settings[$T_LIBRARY_PATH]=[@{@$global_settings[$T_LIBRARY_PATH]}];
  @$project_settings[$T_LIBRARIES]=[@{@$global_settings[$T_LIBRARIES]}];

  @$project[$P_PATH]=$path;
  @$project[$P_SETTINGS]=$project_settings;
  @$project[$P_TARGETS]=[];
}



#####
#
# Global variables
#
#####

my %warnings;

my %templates;

##
# This maps a directory name to a reference to an array listing
# its contents (files and directories)
my %directories;

##
# Contains the list of all projects. This list tells us what are
# the subprojects of the main Makefile and where we have to generate
# Makefiles.
my @projects=();

##
# This is the main project, i.e. the one in the "." directory.
# It may well be empty in which case the main Makefile will only
# call out subprojects.
my @main_project;

##
# Contains the defaults for the include path, etc.
# We store the defaults as if this were a target except that we only
# exploit the defines, include path, library path, library list and misc
# sources fields.
my @global_settings;



#####
#
# Utility functions
#
#####

##
# Cleans up a name to make it an acceptable Makefile
# variable name.
sub canonize($)
{
  my $name=$_[0];

  $name =~ tr/a-zA-Z0-9_/_/c;
  return $name;
}

##
# Returns true is the specified pathname is absolute.
# Note: pathnames that start with a variable '$' or
# '~' are considered absolute.
sub is_absolute($)
{
  my $path=$_[0];

  return ($path =~ /^[\/~\$]/);
}

##
# Retrieves the contents of the specified directory.
# We either get it from the directories hashtable which acts as a
# cache, or use opendir, readdir, closedir and store the result
# in the hashtable.
sub get_directory_contents($)
{
  my $dirname=$_[0];
  my $directory;

  #print "getting the contents of $dirname\n";

  # check for a cached version
  $dirname =~ s+/$++;
  if ($dirname eq "") {
    $dirname=cwd;
  }
  $directory=$directories{$dirname};
  if (defined $directory) {
    #print "->@$directory\n";
    return $directory;
  }

  # Read this directory
  if (opendir(DIRECTORY, "$dirname")) {
    my @files=readdir DIRECTORY;
    closedir(DIRECTORY);
    $directory=\@files;
  } else {
    # Return an empty list
    #print "error: cannot open $dirname\n";
    my @files;
    $directory=\@files;
  }
  #print "->@$directory\n";
  $directories{$dirname}=$directory;
  return $directory;
}

##
# Removes a directory from the cache.
# This is needed if one of its files or subdirectory has been renamed.
sub clear_directory_cache($)
{
    my ($dirname)=@_;
    delete $directories{$dirname};
}


#####
#
# 'Source'-based Project analysis
#
#####

##
# Allows the user to specify makefile and target specific options
# - target: the structure in which to store the results
# - options: the string containing the options
sub source_set_options($$)
{
  my $target=$_[0];
  my $options=$_[1];

  #FIXME: we must deal with escaping of stuff and all
  foreach my $option (split / /,$options) {
    if (@$target[$T_TYPE] == $TT_SETTINGS and $option =~ /^-D/) {
      push @{@$target[$T_DEFINES]},$option;
    } elsif (@$target[$T_TYPE] == $TT_SETTINGS and $option =~ /^-I/) {
      push @{@$target[$T_INCLUDE_PATH]},$option;
    } elsif ($option =~ /^-P/) {
      push @{@$target[$T_DLL_PATH]},"-L$'";
    } elsif ($option =~ /^-i/) {
      push @{@$target[$T_DLLS]},"$'";
    } elsif ($option =~ /^-L/) {
      push @{@$target[$T_LIBRARY_PATH]},$option;
    } elsif ($option =~ /^-l/) {
      push @{@$target[$T_LIBRARIES]},"$'";
    } elsif ($option =~ /^--mfc/) {
      @$target[$T_FLAGS]|=$TF_MFC;
      @$target[$T_FLAGS]&=~$TF_NOMFC;
    } elsif ($option =~ /^--nomfc/) {
      @$target[$T_FLAGS]&=~$TF_MFC;
      @$target[$T_FLAGS]|=$TF_NOMFC;
    } elsif ($option =~ /^--nodlls/) {
      @$target[$T_FLAGS]|=$TF_NODLLS;
    } elsif ($option =~ /^--nomsvcrt/) {
      @$target[$T_FLAGS]|=$TF_NOMSVCRT;
    } else {
      print STDERR "error: unknown option \"$option\"\n";
      return 0;
    }
  }
  return 1;
}

##
# Scans the specified project file to:
# - get a list of targets for this project
# - get some settings
# - get the list of source files
sub source_scan_project_file($$$);
sub source_scan_project_file($$$)
{
    # a reference to the parent's project
    my $parent_project=$_[0];
    # 0 if it is a single project, 1 if it is part of a workspace
    my $is_sub_project=$_[1];
    # the name of the project file, with complete path, or without if in
    # the same directory
    my $filename=$_[2];

    # reference to the project for this file. May not be used
    my $project;
    # list of sources found in the current file
    my @sources_c=();
    my @sources_cxx=();
    my @sources_rc=();
    my @sources_misc=();
    # some more settings
    my $path=dirname($filename);
    my $prj_target_cflags;
    my $prj_target_defines;
    my $prj_target_ldflags;
    my $prj_target_libs;
    my $prj_name;
    my $found_cfg=0;
    my $prj_cfg;
    my $prj_target_type=$TT_GUIEXE;
    my @prj_target_options;

    if (!($path=~/\/$/)) {
        $path.="/";
    }

    $project=$parent_project;
    my $project_settings=@$project[$P_SETTINGS];

    if ($filename =~ /.dsp$/i) {
        # First find out what this project file contains:
        # collect all sources, find targets and settings
        if (!open(FILEI,$filename)) {
            print STDERR "error: unable to open $filename for reading:\n";
            print STDERR "       $!\n";
            return;
        }
        my $sfilet;
        while (<FILEI>) {
            # Remove any trailing CtrlZ, which isn't strictly in the file
            if (/\x1A/) {
                s/\x1A//;
                last if (/^$/)
            }

            # Remove any trailing CrLf
            s/\r\n$/\n/;
            if (!/\n$/) {
                # Make sure all lines are '\n' terminated
                $_ .= "\n";
            }

            if (/^\# Microsoft Developer Studio Project File - Name=\"([^\"]+)/) {
                $prj_name="$1";
                $prj_name=~s/\s+/_/g;
                #print $prj_name;
                next;
            } elsif (/^# TARGTYPE/) {
                if (/[[:space:]]0+x0*101$/) {
                    # Application
                    $prj_target_type=$TT_GUIEXE;
                }elsif (/[[:space:]]0+x0*102$/) {
                    # Dynamic-Link Library
                    $prj_target_type=$TT_DLL;
                }elsif (/[[:space:]]0+x0*103$/) {
                    # Console Application
                    $prj_target_type=$TT_CUIEXE;
                }elsif (/[[:space:]]0+x0*104$/) {
                    # Static Library
                    $prj_target_type=$TT_LIB;
                }
                next;
            } elsif (/^# ADD CPP(.*)/ && $found_cfg==1) {
                $prj_target_cflags=$1;
                @prj_target_options=split(/\s+\//, $prj_target_cflags);
                $prj_target_cflags="";
                foreach ( @prj_target_options ) {
                    if ($_ eq "") {
                        # empty
                    } elsif (/nologo/) {
                        # Suppress Startup Banner and Information Messages
                    } elsif (/^W0$/) {
                        # Turns off all warning messages
                        $prj_target_cflags.="-w ";
                    } elsif (/^W[123]$/) {
                        # Warning Level
                        $prj_target_cflags.="-W ";
                    } elsif (/^W4$/) {
                        # Warning Level
                        $prj_target_cflags.="-Wall ";
                    } elsif (/^WX$/) {
                        # Warnings As Errors
                        $prj_target_cflags.="-Werror ";
                    } elsif (/^Gm$/) {
                        # Enable Minimal Rebuild
                    } elsif (/^GX$/) {
                        # Enable Exception Handling
                        $prj_target_cflags.="-fexceptions ";
                    } elsif (/^Gd$/) {
                        # use cdecl calling convention (default)
                    } elsif (/^Gr$/) {
                        # use fastcall calling convention
                    } elsif (/^Gz$/) {
                        # use stdcall calling convention
                        $prj_target_cflags.="-mrtd ";
                    } elsif (/^Z[d7iI]$/) {
                        # Debug Info
                        $prj_target_cflags.="-g ";
                    } elsif (/^Od$/) {
                        # Disable Optimizations
                        $prj_target_cflags.="-O0 ";
                    } elsif (/^O1$/) {
                        # Minimize Size
                        $prj_target_cflags.="-Os ";
                    } elsif (/^O2$/) {
                        # Maximize Speed
                        $prj_target_cflags.="-O2 ";
                    } elsif (/^Ob0$/) {
                        # Disables inline Expansion
                        $prj_target_cflags.="-fno-inline ";
                    } elsif (/^Ob1$/) {
                        #In-line Function Expansion
                        $prj_target_cflags.="-finline-functions ";
                    } elsif (/^Ob2$/) {
                        # auto In-line Function Expansion
                        $prj_target_cflags.="-finline-functions ";
                    } elsif (/^Ox$/) {
                        # Use maximum optimization
                        $prj_target_cflags.="-O3 ";
                    } elsif (/^Oy$/) {
                        # Frame-Pointer Omission
                        $prj_target_cflags.="-fomit-frame-pointer ";
                    } elsif (/^Oy-$/) {
                        # Frame-Pointer Omission
                        $prj_target_cflags.="-fno-omit-frame-pointer ";
                    } elsif (/^GZ$/) {
                        # Catch Release-Build Errors in Debug Build
                    } elsif (/^M[DLT]d?$/) {
                        # Use Multithreaded Run-Time Library
                    } elsif (/^D\s*\"(.*)\"/) {
                        # Preprocessor Definitions
                        $prj_target_defines.="-D".$1." ";
                    } elsif (/^I\s*\"(.*)\"/) {
                        # Additional Include Directories
                        $sfilet=$1;
                        $sfilet=~s/\\/\//g;
                        my @compinc=split /\/+/, $sfilet;
                        my $realinc=search_from($path, \@compinc);
                        if (defined $realinc) {
                            $sfilet=$realinc;
                        }
                        if ($sfilet=~/^\w:/) {
                            print STDERR "warning: Can't fix path $sfilet\n"
                        } else {
                            push @{@$project_settings[$T_INCLUDE_PATH]},"-I".$sfilet." ";
                        }
                    } elsif (/^U\s*\"(.*)\"/) {
                        # Undefines a previously defined symbol
                        $prj_target_cflags.="-U".$1." ";
                    } elsif (/^Fp/) {
                        # Name .PCH File
                    } elsif (/^F[Rr]/) {
                        # Create .SBR File
                    } elsif (/^YX$/) {
                        # Automatic Use of Precompiled Headers
                    } elsif (/^FD$/) {
                        # Generate File Dependencies
                    } elsif (/^c$/) {
                        # Compile Without Linking
                        # this option is always present and is already specified in the suffix rules
                    } elsif (/^GB$/) {
                        # Blend Optimization
                        $prj_target_cflags.="-D_M_IX86=500 ";
                    } elsif (/^G6$/) {
                        # Pentium Pro Optimization
                        $prj_target_cflags.="-D_M_IX86=600 ";
                    } elsif (/^G5$/) {
                        # Pentium Optimization
                        $prj_target_cflags.="-D_M_IX86=500 ";
                    } elsif (/^G3$/) {
                        # 80386 Optimization
                        $prj_target_cflags.="-D_M_IX86=300 ";
                    } elsif (/^G4$/) {
                        # 80486 Optimization
                        $prj_target_cflags.="-D_M_IX86=400 ";
                    } elsif (/^Yc/) {
                        # Create Precompiled Header
                    } elsif (/^Yu/) {
                        # Use Precompiled Header
                    } elsif (/^Za$/) {
                        # Disable Language Extensions
                        $prj_target_cflags.="-ansi ";
                    } elsif (/^Ze$/) {
                        # Enable Microsoft Extensions
                    } elsif (/^Zm[[:digit:]]+$/) {
                        # Specify Memory Allocation Limit
                    } elsif (/^Zp1?$/) {
                        # Packs structures on 1-byte boundaries
                        $prj_target_cflags.="-fpack-struct ";
                    } elsif (/^Zp(2|4|8|16)$/) {
                        # Struct Member Alignment
                        $prj_target_cflags.="-fpack-struct=".$1;
                    } else {
                        print "C compiler option $_ not implemented\n";
                    }
                }

                #print "\nOptions: $prj_target_cflags\n";
                next;
            } elsif (/^# ADD LINK32(.*)/ && $found_cfg==1) {
                $prj_target_ldflags=$1;
                @prj_target_options=split(/\s+\//, $prj_target_ldflags);
                $prj_target_ldflags="";
                $prj_target_libs=$prj_target_options[0];
                $prj_target_libs=~s/\\/\//g;
                $prj_target_libs=~s/\.lib//g;
                $prj_target_libs=~s/\s+/ -l/g;
                shift (@prj_target_options);
                foreach ( @prj_target_options ) {
                    if ($_ eq "") {
                        # empty
                    } elsif (/^base:(.*)/) {
                        # Base Address
                        $prj_target_ldflags.="--image-base ".$1." ";
                    } elsif (/^debug$/) {
                        # Generate Debug Info
                    } elsif (/^dll$/) {
                        # Build a DLL
                        $prj_target_type=$TT_DLL;
                    } elsif (/^incremental:[[:alpha:]]+$/) {
                        # Link Incrmentally
                    } elsif (/^implib:/) {
                        # Name import library
                    } elsif (/^libpath:\"(.*)\"/) {
                        # Additional Libpath
                        push @{@$project_settings[$T_DLL_PATH]},"-L$1";
                    } elsif (/^machine:[[:alnum:]]+$/) {
                        # Specify Target Platform
                    } elsif (/^map/) {
                        # Generate Mapfile
                        if (/^map:(.*)/) {
                            $prj_target_ldflags.="-Map ".$1." ";
                        } else {
                            $prj_target_ldflags.="-Map ".$prj_name.".map ";
                        }
                    } elsif (/^nologo$/) {
                        # Suppress Startup Banner and Information Messages
                    } elsif (/^out:/) {
                        # Output File Name
                        # may use it as Target?
                    } elsif (/^pdbtype:/) {
                        # Program Database Storage
                    } elsif (/^subsystem:/) {
                        # Specify Subsystem
                    } elsif (/^version:[[:digit:].]+$/) {
                        # Version Information
                    } else {
                        print "Linker option $_ not implemented\n";
                    }
                }
                next;
            } elsif (/^LIB32=/ && $found_cfg==1) {
                #$libflag = 1;
                next;
            } elsif (/^SOURCE=(.*)$/) {
                my @components=split /[\/\\]+/, $1;
                $sfilet=search_from($path, \@components);
                if (!defined $sfilet) { next; }
                if ($sfilet =~ /\.c$/i and $sfilet !~ /\.(dbg|spec)\.c$/) {
                    push @sources_c,$sfilet;
                } elsif ($sfilet =~ /\.cpp$/i) {
                    if ($sfilet =~ /^stdafx.cpp$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
                        push @sources_misc,$sfilet;
                        @$project_settings[$T_FLAGS]|=$TF_MFC;
                    } else {
                        push @sources_cxx,$sfilet;
                    }
                } elsif ($sfilet =~ /\.cxx$/i) {
                    @$project_settings[$T_FLAGS]|=$TF_HASCXX;
                    push @sources_cxx,$sfilet;
                } elsif ($sfilet =~ /\.rc$/i) {
                    push @sources_rc,$sfilet;
                } elsif ($sfilet =~ /\.def$/i) {
                        @$project_settings[$T_FLAGS]|=$TF_HASDEF;
                } elsif ($sfilet =~ /\.(h|hxx|hpp|inl|rc2|dlg)$/i) {
                    push @sources_misc,$sfilet;
                    if ($sfilet =~ /^stdafx.h$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
                        @$project_settings[$T_FLAGS]|=$TF_MFC;
                    }
                }
                next;

            } elsif (/^# (Begin|End) Source File/) {
                # Source-Files already handled
                next;
            } elsif (/^# (Begin|End) Group/) {
                # Groups are ignored
                next;
            } elsif (/^# (Begin|End) Custom Build/) {
                # Custom Builds are ignored
                next;
            } elsif (/^# ADD LIB32 /) {
                #"ARFLAGS=rus"
                next;
            } elsif (/^# Begin Target$/) {
                # Targets are ignored
                next;
            } elsif (/^# End Target$/) {
                # Targets are ignored
                next;
            } elsif (/^!/) {
                if ($found_cfg == 1) {
                    $found_cfg=0;
                }
                if (/if (.*)\(CFG\)" == "(.*)"/i) {
                    if ($2 eq $prj_cfg) {
                        $found_cfg=1;
                    }
                }
                next;
            } elsif (/^CFG=(.*)/i) {
                $prj_cfg=$1;
                next;
            }
                else { # Line recognized
                # print "|\n";
            }
        }
        close(FILEI);

        push @{@$project_settings[$T_LIBRARIES]},$prj_target_libs;
        push @{@$project_settings[$T_CEXTRA]},$prj_target_cflags;
        push @{@$project_settings[$T_CXXEXTRA]},$prj_target_cflags;
        push @{@$project_settings[$T_DEFINES]},$prj_target_defines;
        push @{@$project_settings[$T_LDFLAGS]},$prj_target_ldflags;
    } elsif ($filename =~ /.vcproj$/i) {
        eval {
            require XML::LibXML;
        };
        if ($@) {
            die "Error: You need the libxml package (deb: libxml-libxml-perl, rpm: perl-libxml-perl)";
        }

        my $xmlparser = XML::LibXML->new();
        my $project_xml = $xmlparser->parse_file($filename);
        my $sfilet;
        my $configt;

        foreach my $vc_project ($project_xml->findnodes('/VisualStudioProject')) {
            foreach my $vc_project_attr ($vc_project->attributes) {
                if ($vc_project_attr->getName eq "Name") {
                    $prj_name=$vc_project_attr->getValue;
                    $prj_name=~s/\s+/_/g;
                    last;
                }
            }
        }

        for (my $flevel = 0; $flevel <= 5; $flevel++) {
            foreach my $vc_file ($project_xml->findnodes('/VisualStudioProject/Files/'.('Filter/' x $flevel).'File')) {
                foreach my $vc_file_attr ($vc_file->attributes) {
                    if ($vc_file_attr->getName eq "RelativePath") {
                        $sfilet = $vc_file_attr->getValue;
                        $sfilet=~s/\\\\/\\/g; #remove double backslash
                        $sfilet=~s/^\.\\//; #remove starting 'this directory'
                        $sfilet=~s/\\/\//g; #make slashes out of backslashes
                        my @compsrc=split(/\/+/, $sfilet);
                        my $realsrc=search_from($path, \@compsrc);
                        if (defined $realsrc) {
                            $sfilet=$realsrc;
                        }
                        if ($sfilet =~ /\.c$/i and $sfilet !~ /\.(dbg|spec)\.c$/) {
                            push @sources_c,$sfilet;
                        } elsif ($sfilet =~ /\.cpp$/i) {
                            if ($sfilet =~ /^stdafx.cpp$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
                                push @sources_misc,$sfilet;
                                @$project_settings[$T_FLAGS]|=$TF_MFC;
                            } else {
                                push @sources_cxx,$sfilet;
                            }
                        } elsif ($sfilet =~ /\.cxx$/i) {
                            @$project_settings[$T_FLAGS]|=$TF_HASCXX;
                            push @sources_cxx,$sfilet;
                        } elsif ($sfilet =~ /\.rc$/i) {
                            push @sources_rc,$sfilet;
                        } elsif ($sfilet =~ /\.def$/i) {
                            @$project_settings[$T_FLAGS]|=$TF_HASDEF;
                        } elsif ($sfilet =~ /\.(h|hxx|hpp|inl|rc2|dlg)$/i) {
                            push @sources_misc,$sfilet;
                            if ($sfilet =~ /^stdafx.h$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
                                @$project_settings[$T_FLAGS]|=$TF_MFC;
                            }
                        }
                    }
                }
            }
        }

        my @vc_configurations = $project_xml->findnodes('/VisualStudioProject/Configurations/Configuration');
        my $vc_configuration = $vc_configurations[0];
        foreach my $vc_configuration_attr ($vc_configuration->attributes) {
            if ($vc_configuration_attr->getName eq "ConfigurationType") {
                if ($vc_configuration_attr->getValue==1) {
                    $prj_target_type=$TT_GUIEXE; # Application
                } elsif ($vc_configuration_attr->getValue==2) {
                    $prj_target_type=$TT_DLL; # Dynamic-Link Library
                } elsif ($vc_configuration_attr->getValue==4) {
                    $prj_target_type=$TT_LIB; # Static Library
                }
            }
        }

        foreach my $vc_configuration_tools ($vc_configuration->findnodes('Tool')) {
            my @find_tool = $vc_configuration_tools->attributes;
            if ($find_tool[0]->getValue eq "VCCLCompilerTool") {
                foreach my $vc_compiler_tool ($vc_configuration_tools->attributes) {
                    if ($vc_compiler_tool->getName eq "Optimization") {$prj_target_cflags.="-O".$vc_compiler_tool->getValue." ";}
                    if ($vc_compiler_tool->getName eq "WarningLevel") {
                        if ($vc_compiler_tool->getValue==0) {
                            $prj_target_cflags.="-w ";
                        } elsif ($vc_compiler_tool->getValue<4) {
                            $prj_target_cflags.="-W ";
                        } elsif ($vc_compiler_tool->getValue==4) {
                            $prj_target_cflags.="-Wall ";
                        } elsif ($vc_compiler_tool->getValue eq "X") {
                            $prj_target_cflags.="-Werror ";
                        }
                    }
                    if ($vc_compiler_tool->getName eq "PreprocessorDefinitions") {
                        $configt=$vc_compiler_tool->getValue;
                        $configt=~s/;\s*/ -D/g;
                        $prj_target_defines.="-D".$configt." ";
                    }
                    if ($vc_compiler_tool->getName eq "AdditionalIncludeDirectories") {
                        $configt=$vc_compiler_tool->getValue;
                        $configt=~s/\\/\//g;
                        my @addincl = split(/\s*;\s*/, $configt);
                        foreach $configt (@addincl) {
                            my @compinc=split(/\/+/, $configt);
                            my $realinc=search_from($path, \@compinc);
                            if (defined $realinc) {
                                $configt=$realinc;
                            }
                            push @{@$project_settings[$T_INCLUDE_PATH]},"-I".$configt;
                        }
                    }
                }
            }
            if ($find_tool[0]->getValue eq "VCLinkerTool") {
                foreach my $vc_linker_tool ($vc_configuration_tools->attributes) {
                    if ($vc_linker_tool->getName eq "AdditionalDependencies") {
                        $prj_target_libs=" ".$vc_linker_tool->getValue;
                        $prj_target_libs=~s/\\/\//g;
                        $prj_target_libs=~s/\.lib//g;
                        $prj_target_libs=~s/\s+/ -l/g;
                    }
                }
            }
        }

        push @{@$project_settings[$T_LIBRARIES]},$prj_target_libs;
        push @{@$project_settings[$T_CEXTRA]},$prj_target_cflags;
        push @{@$project_settings[$T_CXXEXTRA]},$prj_target_cflags;
        push @{@$project_settings[$T_DEFINES]},$prj_target_defines;
    } else {
        print STDERR "File format not supported for file: $filename\n";
        return;
    }

    # Add this project to the project list, except for
    # the main project which is already in the list.
    if ($is_sub_project == 1) {
        push @projects,$project;
    }

    # Ask for project-wide options
    if ($opt_ask_project_options == $OPT_ASK_YES) {
        my $flag_desc="";
        if ((@$project_settings[$T_FLAGS] & $TF_MFC)!=0) {
            $flag_desc="mfc";
        }
        print "* Type any project-wide options (-D/-I/-P/-i/-L/-l/--mfc),\n";
        if (defined $flag_desc) {
            print "* (currently $flag_desc)\n";
        }
        print "* or 'skip' to skip the target specific options,\n";
        print "* or 'never' to not be asked this question again:\n";
        while (1) {
            my $options=<STDIN>;
            chomp $options;
            if ($options eq "skip") {
                $opt_ask_target_options=$OPT_ASK_SKIP;
                last;
            } elsif ($options eq "never") {
                $opt_ask_project_options=$OPT_ASK_NO;
                last;
            } elsif (source_set_options($project_settings,$options)) {
                last;
            }
            print "Please re-enter the options:\n";
        }
    }

    # Create the target...
    my $target=[];
    target_init($target);

    if ($prj_target_type==$TT_GUIEXE or $prj_target_type==$TT_CUIEXE) {
        $prj_name=lc($prj_name.".exe");
        @$target[$T_TYPE]=$opt_target_type;
        push @{@$target[$T_LDFLAGS]},(@$target[$T_TYPE] == $TT_CUIEXE ? "-mconsole" : "-mwindows");
    } elsif ($prj_target_type==$TT_LIB) {
        $prj_name=lc("lib".$prj_name.".a");
        @$target[$T_TYPE]=$TT_LIB;
        push @{@$target[$T_ARFLAGS]},("rc");
    } else {
        $prj_name=lc($prj_name.".dll");
        @$target[$T_TYPE]=$TT_DLL;
        my $canon=canonize($prj_name);
        if (@$project_settings[$T_FLAGS] & $TF_HASDEF) {
            push @{@$target[$T_LDFLAGS]},("-shared","\$(${canon}_MODULE:.dll=.def)");
        } else {
            push @{@$target[$T_LDFLAGS]},("-shared","\$(${canon}_MODULE:.dll=.spec)");
        }
    }

    @$target[$T_NAME]=$prj_name;
    @$target[$T_FLAGS]|=@$project_settings[$T_FLAGS];

    # This is the default link list of Visual Studio
    my @std_imports=qw(odbc32 ole32 oleaut32 winspool odbccp32);
    my @std_libraries=qw(uuid);
    if ((@$target[$T_FLAGS] & $TF_NODLLS) == 0) {
        @$target[$T_DLLS]=\@std_imports;
        @$target[$T_LIBRARIES]=\@std_libraries;
    } else {
        @$target[$T_DLLS]=[];
        @$target[$T_LIBRARIES]=[];
    }
    if ((@$target[$T_FLAGS] & $TF_NOMSVCRT) == 0) {
        push @{@$target[$T_LDFLAGS]},"-mno-cygwin";
        if ($opt_arch != $OPT_ARCH_DEFAULT) {
            push @{@$target[$T_LDFLAGS]},"-m$opt_arch";
        }
    }
    push @{@$project[$P_TARGETS]},$target;

    # Ask for target-specific options
    if ($opt_ask_target_options == $OPT_ASK_YES) {
        my $flag_desc="";
        if ((@$target[$T_FLAGS] & $TF_MFC)!=0) {
            $flag_desc=" (mfc";
        }
        if ($flag_desc ne "") {
            $flag_desc.=")";
        }
        print "* Specify any link option (-P/-i/-L/-l/--mfc) specific to the target\n";
        print "* \"$prj_name\"$flag_desc or 'never' to not be asked this question again:\n";
        while (1) {
        my $options=<STDIN>;
        chomp $options;
        if ($options eq "never") {
            $opt_ask_target_options=$OPT_ASK_NO;
            last;
        } elsif (source_set_options($target,$options)) {
            last;
        }
        print "Please re-enter the options:\n";
        }
    }
    if (@$target[$T_FLAGS] & $TF_MFC) {
        @$project_settings[$T_FLAGS]|=$TF_MFC;
        push @{@$target[$T_DLL_PATH]},"\$(MFC_LIBRARY_PATH)";
        push @{@$target[$T_DLLS]},"mfc.dll";
        # FIXME: Link with the MFC in the Unix sense, until we
        # start exporting the functions properly.
        push @{@$target[$T_LIBRARY_PATH]},"\$(MFC_LIBRARY_PATH)";
        push @{@$target[$T_LIBRARIES]},"mfc";
    }

    # Match sources...
    push @{@$target[$T_SOURCES_C]},@{@$project_settings[$T_SOURCES_C]},@sources_c;
    @$project_settings[$T_SOURCES_C]=[];
    @sources_c=();
    push @{@$target[$T_SOURCES_CXX]},@{@$project_settings[$T_SOURCES_CXX]},@sources_cxx;
    @$project_settings[$T_SOURCES_CXX]=[];
    @sources_cxx=();
    push @{@$target[$T_SOURCES_RC]},@{@$project_settings[$T_SOURCES_RC]},@sources_rc;
    @$project_settings[$T_SOURCES_RC]=[];
    @sources_rc=();
    push @{@$target[$T_SOURCES_MISC]},@{@$project_settings[$T_SOURCES_MISC]},@sources_misc;
    @$project_settings[$T_SOURCES_MISC]=[];
    @sources_misc=();

    @$target[$T_SOURCES_C]=[sort @{@$target[$T_SOURCES_C]}];
    @$target[$T_SOURCES_CXX]=[sort @{@$target[$T_SOURCES_CXX]}];
    @$target[$T_SOURCES_RC]=[sort @{@$target[$T_SOURCES_RC]}];
    @$target[$T_SOURCES_MISC]=[sort @{@$target[$T_SOURCES_MISC]}];

    if ($opt_ask_target_options == $OPT_ASK_SKIP) {
        $opt_ask_target_options=$OPT_ASK_YES;
    }

    if ((@$project_settings[$T_FLAGS] & $TF_NOMSVCRT) == 0) {
        push @{@$project_settings[$T_CEXTRA]},"-mno-cygwin";
        if ($opt_arch != $OPT_ARCH_DEFAULT) {
            push @{@$project_settings[$T_CEXTRA]},"-m$opt_arch";
            push @{@$project_settings[$T_CXXEXTRA]},"-m$opt_arch";
        }
    }

    if (@$project_settings[$T_FLAGS] & $TF_MFC) {
        push @{@$project_settings[$T_INCLUDE_PATH]},"\$(MFC_INCLUDE_PATH)";
    }
    # The sources that did not match, if any, go to the extra
    # source list of the project settings
    foreach my $source (@sources_c) {
        if ($source ne "") {
            push @{@$project_settings[$T_SOURCES_C]},$source;
        }
    }
    @$project_settings[$T_SOURCES_C]=[sort @{@$project_settings[$T_SOURCES_C]}];
    foreach my $source (@sources_cxx) {
        if ($source ne "") {
            push @{@$project_settings[$T_SOURCES_CXX]},$source;
        }
    }
    @$project_settings[$T_SOURCES_CXX]=[sort @{@$project_settings[$T_SOURCES_CXX]}];
    foreach my $source (@sources_rc) {
        if ($source ne "") {
            push @{@$project_settings[$T_SOURCES_RC]},$source;
        }
    }
    @$project_settings[$T_SOURCES_RC]=[sort @{@$project_settings[$T_SOURCES_RC]}];
    foreach my $source (@sources_misc) {
        if ($source ne "") {
            push @{@$project_settings[$T_SOURCES_MISC]},$source;
        }
    }
    @$project_settings[$T_SOURCES_MISC]=[sort @{@$project_settings[$T_SOURCES_MISC]}];
}

##
# Scans the specified workspace file to find the project files
sub source_scan_workspace_file($);
sub source_scan_workspace_file($)
{
    my $filename=$_[0];
    my $path=dirname($filename);
    my @components;

    if (! -e $filename) {
        return;
    }

    if (!open(FILEIWS,$filename)) {
        print STDERR "error: unable to open $filename for reading:\n";
        print STDERR "       $!\n";
        return;
    }

    my $prj_name;
    my $prj_path;

    if ($filename =~ /.dsw$/i) {
        while (<FILEIWS>) {
            # Remove any trailing CrLf
            s/\r\n$/\n/;

            # catch a project definition
            if (/^Project:\s\"(.*)\"=(.*)\s-/) {
                $prj_name=$1;
                $prj_path=$2;
                @components=split /[\/\\]+/, $prj_path;
                $prj_path=search_from($path, \@components);
                print "Name: $prj_name\nPath: $prj_path\n";
                source_scan_project_file(\@main_project,1,$prj_path);
                next;
            } elsif (/^#/) {
                # ignore Comments
            } elsif (/^Global:/) {
                # ignore the Global section
            } elsif (/\w:/) {
                print STDERR "unknown section $_\n";
            } elsif (/^Microsoft(.*)Studio(.*)File,\sFormat Version\s(.*)/) {
                print "\nFileversion: $3\n";
            }
        }
        close(FILEIWS);
    } elsif ($filename =~ /.sln$/i) {
        while (<FILEIWS>) {
            # Remove any trailing CrLf
            s/\r\n$/\n/;

            # catch a project definition
            if (/^Project(.*)=\s*"(.*)",\s*"(.*)",\s*"(.*)"/) {
                $prj_name=$2;
                $prj_path=$3;
                if ($prj_path eq "Solution Items") { next; }
                @components=split /[\/\\]+/, $3;
                $prj_path=search_from($path, \@components);
                print "Name: $prj_name\nPath: $prj_path\n";
                source_scan_project_file(\@main_project,1,$prj_path);
                next;
            } elsif (/^Microsoft(.*)Studio(.*)File,\sFormat Version\s(.*)/) {
                print "\nFileversion: $3\n";
            }
        }
        close(FILEIWS);
    }

    @projects=sort { @$a[$P_PATH] cmp @$b[$P_PATH] } @projects;
}

##
# Scans the specified directory to:
# - see if we should create a Makefile in this directory. We normally do
#   so if we find a project file and sources
# - get a list of targets for this directory
# - get the list of source files
sub source_scan_directory($$$$);
sub source_scan_directory($$$$)
{
  # a reference to the parent's project
  my $parent_project=$_[0];
  # the full relative path to the current directory, including a
  # trailing '/', or an empty string if this is the top level directory
  my $path=$_[1];
  # the name of this directory, including a trailing '/', or an empty
  # string if this is the top level directory
  my $dirname=$_[2];
  # if set then no targets will be looked for and the sources will all
  # end up in the parent_project's 'misc' bucket
  my $no_target=$_[3];

  # reference to the project for this directory. May not be used
  my $project;
  # list of targets found in the 'current' directory
  my %targets;
  # list of sources found in the current directory
  my @sources_c=();
  my @sources_cxx=();
  my @sources_rc=();
  my @sources_misc=();
  # true if this directory contains a Windows project
  my $has_win_project=0;
  # true if this directory contains headers
  my $has_headers=0;
  # If we don't find any executable/library then we might make up targets
  # from the list of .dsp/.mak files we find since they usually have the
  # same name as their target.
  my @prj_files=();
  my @mak_files=();

  if (defined $opt_single_target or $dirname eq "") {
    # Either there is a single target and thus a single project,
    # or we are in the top level directory for which a project
    # already exists
    $project=$parent_project;
  } else {
    $project=[];
    project_init($project, $path, \@global_settings);
  }
  my $project_settings=@$project[$P_SETTINGS];

  # First find out what this directory contains:
  # collect all sources, targets and subdirectories
  my $directory=get_directory_contents($path);
  foreach my $dentry (@$directory) {
    if ($dentry =~ /^\./) {
      next;
    }
    my $fullentry="$path$dentry";
    if (-d "$fullentry") {
      if ($dentry =~ /^(Release|Debug)/i) {
	# These directories are often used to store the object files and the
	# resulting executable/library. They should not contain anything else.
        my @candidates=grep /\.(exe|dll|lib)$/i, @{get_directory_contents("$fullentry")};
        foreach my $candidate (sort @candidates) {
            my $dlldup = $candidate;
            $dlldup =~ s/\.lib$/.dll/;
            if ($candidate =~ /\.lib$/ and $targets{$dlldup})
            {
                # Often lib files are created together with dll files, even if the dll file is the
                # real target.
                next;
            }
            $targets{$candidate}=1;
        }
      } elsif ($dentry =~ /^include/i) {
        # This directory must contain headers we're going to need
        push @{@$project_settings[$T_INCLUDE_PATH]},"-I$dentry";
        source_scan_directory($project,"$fullentry/","$dentry/",1);
      } else {
	# Recursively scan this directory. Any source file that cannot be
	# attributed to a project in one of the subdirectories will be
	# attributed to this project.
	source_scan_directory($project,"$fullentry/","$dentry/",$no_target);
      }
    } elsif (-f "$fullentry") {
      if ($dentry =~ /\.(exe|dll|lib)$/i) {
	$targets{$dentry}=1;
      } elsif ($dentry =~ /\.c$/i and $dentry !~ /\.(dbg|spec)\.c$/) {
	push @sources_c,"$dentry";
      } elsif ($dentry =~ /\.cpp$/i) {
	if ($dentry =~ /^stdafx.cpp$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
	  push @sources_misc,"$dentry";
	  @$project_settings[$T_FLAGS]|=$TF_MFC;
	} else {
	  push @sources_cxx,"$dentry";
	}
      } elsif ($dentry =~ /\.cxx$/i) {
          @$project_settings[$T_FLAGS]|=$TF_HASCXX;
          push @sources_cxx,"$dentry";
      } elsif ($dentry =~ /\.rc$/i) {
	push @sources_rc,"$dentry";
      } elsif ($dentry =~ /\.def$/i) {
	@$project_settings[$T_FLAGS]|=$TF_HASDEF;
      } elsif ($dentry =~ /\.(h|hxx|hpp|inl|rc2|dlg)$/i) {
	$has_headers=1;
	push @sources_misc,"$dentry";
	if ($dentry =~ /^stdafx.h$/i && !(@$project_settings[$T_FLAGS] & $TF_NOMFC)) {
	  @$project_settings[$T_FLAGS]|=$TF_MFC;
	}
      } elsif ($dentry =~ /\.(dsp|vcproj)$/i) {
	push @prj_files,"$dentry";
	$has_win_project=1;
      } elsif ($dentry =~ /\.mak$/i) {
	push @mak_files,"$dentry";
	$has_win_project=1;
      } elsif ($dentry =~ /^makefile/i) {
	$has_win_project=1;
      }
    }
  }

  if ($has_headers) {
    push @{@$project_settings[$T_INCLUDE_PATH]},"-I.";
  }
  # If we have a single target then all we have to do is assign
  # all the sources to it and we're done
  # FIXME: does this play well with the --interactive mode?
  if ($opt_single_target) {
    my $target=@{@$project[$P_TARGETS]}[0];
    push @{@$target[$T_SOURCES_C]},map "$path$_",@sources_c;
    push @{@$target[$T_SOURCES_CXX]},map "$path$_",@sources_cxx;
    push @{@$target[$T_SOURCES_RC]},map "$path$_",@sources_rc;
    push @{@$target[$T_SOURCES_MISC]},map "$path$_",@sources_misc;
    return;
  }
  if ($no_target) {
    my $parent_settings=@$parent_project[$P_SETTINGS];
    push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_c;
    push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_cxx;
    push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_rc;
    push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_misc;
    push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@{@$project_settings[$T_SOURCES_MISC]};
    return;
  }

  my $source_count=@sources_c+@sources_cxx+@sources_rc+
                   @{@$project_settings[$T_SOURCES_C]}+
                   @{@$project_settings[$T_SOURCES_CXX]}+
                   @{@$project_settings[$T_SOURCES_RC]};
  if ($source_count == 0) {
    # A project without real sources is not a project, get out!
    if ($project!=$parent_project) {
      my $parent_settings=@$parent_project[$P_SETTINGS];
      push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_misc;
      push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@{@$project_settings[$T_SOURCES_MISC]};
    }
    return;
  }
  #print "targets=",%targets,"\n";
  #print "target_count=$target_count\n";
  #print "has_win_project=$has_win_project\n";
  #print "dirname=$dirname\n";

  my $target_count;
  if (($has_win_project != 0) or ($dirname eq "")) {
    # Deal with cases where we could not find any executable/library, and
    # thus have no target, although we did find some sort of windows project.
    $target_count=keys %targets;
    if ($target_count == 0) {
      # Try to come up with a target list based on .dsp/.mak files
      my $prj_list;
      if (@prj_files > 0) {
        print "Projectfile found! You might want to try using it directly.\n";
        $prj_list=\@prj_files;
      } else {
        $prj_list=\@mak_files;
      }
      foreach my $filename (@$prj_list) {
	$filename =~ s/\.(dsp|vcproj|mak)$//i;
	if ($opt_target_type == $TT_DLL) {
	  $filename = "$filename.dll";
	}
	$targets{$filename}=1;
      }
      $target_count=keys %targets;
      if ($target_count == 0) {
	# Still nothing, try the name of the directory
	my $name;
	if ($dirname eq "") {
	  # Bad luck, this is the top level directory!
	  $name=(split /\//, cwd)[-1];
	} else {
	  $name=$dirname;
	  # Remove the trailing '/'. Also eliminate whatever is after the last
	  # '.' as it is likely to be meaningless (.orig, .new, ...)
	  $name =~ s+(/|\.[^.]*)$++;
	  if ($name eq "src") {
	    # 'src' is probably a subdirectory of the real project directory.
	    # Try again with the parent (if any).
	    my $parent=$path;
	    if ($parent =~ s+([^/]*)/[^/]*/$+$1+) {
	      $name=$parent;
	    } else {
	      $name=(split /\//, cwd)[-1];
	    }
	  }
	}
	$name =~ s+(/|\.[^.]*)$++;
	if ($opt_target_type == $TT_DLL) {
	  $name = canonize($name).".dll";
	} elsif ($opt_target_type == $TT_LIB) {
	  $name = "lib".canonize($name).".a";
	} else {
	  $name = canonize($name).".exe";
	}
	$targets{$name}=1;
      }
    }

    # Ask confirmation to the user if he wishes so
    if ($opt_is_interactive == $OPT_ASK_YES) {
      my $target_list=join " ",keys %targets;
      print "\n*** In ",($path?$path:"./"),"\n";
      print "* winemaker found the following list of (potential) targets\n";
      print "*   $target_list\n";
      print "* Type enter to use it as is, your own comma-separated list of\n";
      print "* targets, 'none' to assign the source files to a parent directory,\n";
      print "* or 'ignore' to ignore everything in this directory tree.\n";
      print "* Target list:\n";
      $target_list=<STDIN>;
      chomp $target_list;
      if ($target_list eq "") {
	# Keep the target list as is, i.e. do nothing
      } elsif ($target_list eq "none") {
	# Empty the target list
	undef %targets;
      } elsif ($target_list eq "ignore") {
	# Ignore this subtree altogether
	return;
      } else {
	undef %targets;
	foreach my $target (split /,/,$target_list) {
	  $target =~ s+^\s*++;
	  $target =~ s+\s*$++;
	  $targets{$target}=1;
	}
      }
    }
  }

  # If we have no project at this level, then transfer all
  # the sources to the parent project
  $target_count=keys %targets;
  if ($target_count == 0) {
    if ($project!=$parent_project) {
      my $parent_settings=@$parent_project[$P_SETTINGS];
      push @{@$parent_settings[$T_SOURCES_C]},map "$dirname$_",@sources_c;
      push @{@$parent_settings[$T_SOURCES_CXX]},map "$dirname$_",@sources_cxx;
      push @{@$parent_settings[$T_SOURCES_RC]},map "$dirname$_",@sources_rc;
      push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@sources_misc;
      push @{@$parent_settings[$T_SOURCES_MISC]},map "$dirname$_",@{@$project_settings[$T_SOURCES_MISC]};
    }
    return;
  }

  # Otherwise add this project to the project list, except for
  # the main project which is already in the list.
  if ($dirname ne "") {
    push @projects,$project;
  }

  # Ask for project-wide options
  if ($opt_ask_project_options == $OPT_ASK_YES) {
    my $flag_desc="";
    if ((@$project_settings[$T_FLAGS] & $TF_MFC)!=0) {
      $flag_desc="mfc";
    }
    print "* Type any project-wide options (-D/-I/-P/-i/-L/-l/--mfc),\n";
    if (defined $flag_desc) {
      print "* (currently $flag_desc)\n";
    }
    print "* or 'skip' to skip the target specific options,\n";
    print "* or 'never' to not be asked this question again:\n";
    while (1) {
      my $options=<STDIN>;
      chomp $options;
      if ($options eq "skip") {
        $opt_ask_target_options=$OPT_ASK_SKIP;
        last;
      } elsif ($options eq "never") {
        $opt_ask_project_options=$OPT_ASK_NO;
        last;
      } elsif (source_set_options($project_settings,$options)) {
        last;
      }
      print "Please re-enter the options:\n";
    }
  }

  # - Create the targets
  # - Check if we have both libraries and programs
  # - Match each target with source files (sort in reverse
  #   alphabetical order to get the longest matches first)
  my @local_dlls=();
  my @local_libs=();
  my @local_depends=();
  my @exe_list=();
  foreach my $target_name (map (lc, (sort { $b cmp $a } keys %targets))) {
    # Create the target...
    my $target=[];
    target_init($target);
    @$target[$T_NAME]=$target_name;
    @$target[$T_FLAGS]|=@$project_settings[$T_FLAGS];
    if ($target_name =~ /\.dll$/) {
      @$target[$T_TYPE]=$TT_DLL;
      push @local_depends,"$target_name.so";
      push @local_dlls,$target_name;
      my $canon=canonize($target_name);
      if (@$project_settings[$T_FLAGS] & $TF_HASDEF) {
          push @{@$target[$T_LDFLAGS]},("-shared","\$(${canon}_MODULE:.dll=.def)");
      } else {
          push @{@$target[$T_LDFLAGS]},("-shared","\$(${canon}_MODULE:.dll=.spec)");
      }
    } elsif ($target_name =~ /\.lib$/) {
      $target_name =~ s/(.*)\.lib/lib$1.a/;
      @$target[$T_NAME]=$target_name;
      @$target[$T_TYPE]=$TT_LIB;
      push @local_depends,"$target_name";
      push @local_libs,$target_name;
      push @{@$target[$T_ARFLAGS]},("rc");
    } elsif ($target_name =~ /\.a$/) {
      @$target[$T_NAME]=$target_name;
      @$target[$T_TYPE]=$TT_LIB;
      push @local_depends,"$target_name";
      push @local_libs,$target_name;
      push @{@$target[$T_ARFLAGS]},("rc");
    } else {
      @$target[$T_TYPE]=$opt_target_type;
      push @exe_list,$target;
      push @{@$target[$T_LDFLAGS]},(@$target[$T_TYPE] == $TT_CUIEXE ? "-mconsole" : "-mwindows");
    }
    my $basename=$target_name;
    $basename=~ s/\.(dll|exe)$//i;
    # This is the default link list of Visual Studio
    my @std_imports=qw(odbc32 ole32 oleaut32 winspool odbccp32);
    my @std_libraries=qw(uuid);
    if ((@$target[$T_FLAGS] & $TF_NODLLS) == 0) {
      @$target[$T_DLLS]=\@std_imports;
      @$target[$T_LIBRARIES]=\@std_libraries;
    } else {
      @$target[$T_DLLS]=[];
      @$target[$T_LIBRARIES]=[];
    }
    if ((@$target[$T_FLAGS] & $TF_NOMSVCRT) == 0) {
      push @{@$target[$T_LDFLAGS]},"-mno-cygwin";
      if ($opt_arch != $OPT_ARCH_DEFAULT) {
        push @{@$target[$T_LDFLAGS]},"-m$opt_arch";
      }
    }
    push @{@$project[$P_TARGETS]},$target;

    # Ask for target-specific options
    if ($opt_ask_target_options == $OPT_ASK_YES) {
      my $flag_desc="";
      if ((@$target[$T_FLAGS] & $TF_MFC)!=0) {
	$flag_desc=" (mfc";
      }
      if ($flag_desc ne "") {
	$flag_desc.=")";
      }
      print "* Specify any link option (-P/-i/-L/-l/--mfc) specific to the target\n";
      print "* \"$target_name\"$flag_desc or 'never' to not be asked this question again:\n";
      while (1) {
        my $options=<STDIN>;
        chomp $options;
        if ($options eq "never") {
          $opt_ask_target_options=$OPT_ASK_NO;
          last;
        } elsif (source_set_options($target,$options)) {
          last;
        }
        print "Please re-enter the options:\n";
      }
    }
    if (@$target[$T_FLAGS] & $TF_MFC) {
      @$project_settings[$T_FLAGS]|=$TF_MFC;
      push @{@$target[$T_DLL_PATH]},"\$(MFC_LIBRARY_PATH)";
      push @{@$target[$T_DLLS]},"mfc.dll";
      # FIXME: Link with the MFC in the Unix sense, until we
      # start exporting the functions properly.
      push @{@$target[$T_LIBRARY_PATH]},"\$(MFC_LIBRARY_PATH)";
      push @{@$target[$T_LIBRARIES]},"mfc";
    }

    # Match sources...
    if ($target_count == 1) {
      push @{@$target[$T_SOURCES_C]},@{@$project_settings[$T_SOURCES_C]},@sources_c;
      @$project_settings[$T_SOURCES_C]=[];
      @sources_c=();

      push @{@$target[$T_SOURCES_CXX]},@{@$project_settings[$T_SOURCES_CXX]},@sources_cxx;
      @$project_settings[$T_SOURCES_CXX]=[];
      @sources_cxx=();

      push @{@$target[$T_SOURCES_RC]},@{@$project_settings[$T_SOURCES_RC]},@sources_rc;
      @$project_settings[$T_SOURCES_RC]=[];
      @sources_rc=();

      push @{@$target[$T_SOURCES_MISC]},@{@$project_settings[$T_SOURCES_MISC]},@sources_misc;
      # No need for sorting these sources
      @$project_settings[$T_SOURCES_MISC]=[];
      @sources_misc=();
    } else {
      foreach my $source (@sources_c) {
	if ($source =~ /^$basename/i) {
	  push @{@$target[$T_SOURCES_C]},$source;
	  $source="";
	}
      }
      foreach my $source (@sources_cxx) {
	if ($source =~ /^$basename/i) {
	  push @{@$target[$T_SOURCES_CXX]},$source;
	  $source="";
	}
      }
      foreach my $source (@sources_rc) {
	if ($source =~ /^$basename/i) {
	  push @{@$target[$T_SOURCES_RC]},$source;
	  $source="";
	}
      }
      foreach my $source (@sources_misc) {
	if ($source =~ /^$basename/i) {
	  push @{@$target[$T_SOURCES_MISC]},$source;
	  $source="";
	}
      }
    }
    @$target[$T_SOURCES_C]=[sort @{@$target[$T_SOURCES_C]}];
    @$target[$T_SOURCES_CXX]=[sort @{@$target[$T_SOURCES_CXX]}];
    @$target[$T_SOURCES_RC]=[sort @{@$target[$T_SOURCES_RC]}];
    @$target[$T_SOURCES_MISC]=[sort @{@$target[$T_SOURCES_MISC]}];
  }
  if ($opt_ask_target_options == $OPT_ASK_SKIP) {
    $opt_ask_target_options=$OPT_ASK_YES;
  }

  if ((@$project_settings[$T_FLAGS] & $TF_NOMSVCRT) == 0) {
    push @{@$project_settings[$T_CEXTRA]},"-mno-cygwin";
      if ($opt_arch != $OPT_ARCH_DEFAULT) {
        push @{@$project_settings[$T_CEXTRA]},"-m$opt_arch";
        push @{@$project_settings[$T_CXXEXTRA]},"-m$opt_arch";
      }
  }

  if (@$project_settings[$T_FLAGS] & $TF_MFC) {
    push @{@$project_settings[$T_INCLUDE_PATH]},"\$(MFC_INCLUDE_PATH)";
  }
  # The sources that did not match, if any, go to the extra
  # source list of the project settings
  foreach my $source (@sources_c) {
    if ($source ne "") {
      push @{@$project_settings[$T_SOURCES_C]},$source;
    }
  }
  @$project_settings[$T_SOURCES_C]=[sort @{@$project_settings[$T_SOURCES_C]}];
  foreach my $source (@sources_cxx) {
    if ($source ne "") {
      push @{@$project_settings[$T_SOURCES_CXX]},$source;
    }
  }
  @$project_settings[$T_SOURCES_CXX]=[sort @{@$project_settings[$T_SOURCES_CXX]}];
  foreach my $source (@sources_rc) {
    if ($source ne "") {
      push @{@$project_settings[$T_SOURCES_RC]},$source;
    }
  }
  @$project_settings[$T_SOURCES_RC]=[sort @{@$project_settings[$T_SOURCES_RC]}];
  foreach my $source (@sources_misc) {
    if ($source ne "") {
      push @{@$project_settings[$T_SOURCES_MISC]},$source;
    }
  }
  @$project_settings[$T_SOURCES_MISC]=[sort @{@$project_settings[$T_SOURCES_MISC]}];

  # Finally if we are building both libraries and programs in
  # this directory, then the programs should be linked with all
  # the libraries
  if (@local_dlls > 0 and @exe_list > 0) {
    foreach my $target (@exe_list) {
      push @{@$target[$T_DLL_PATH]},"-L.";
      push @{@$target[$T_DLLS]},@local_dlls;
    }
  }
  if (@local_libs > 0 and @exe_list > 0) {
    foreach my $target (@exe_list) {
      push @{@$target[$T_LIBRARY_PATH]},"-L.";
      push @{@$target[$T_LIBRARIES]},@local_libs;
    }
  }
}

##
# Scan the source directories in search of things to build
sub source_scan()
{
  # If there's a single target then this is going to be the default target
  if (defined $opt_single_target) {
    # Create the main target
    my $main_target=[];
    target_init($main_target);
    @$main_target[$T_NAME]=$opt_single_target;
    @$main_target[$T_TYPE]=$opt_target_type;

    # Add it to the list
    push @{$main_project[$P_TARGETS]},$main_target;
  }

  # The main directory is always going to be there
  push @projects,\@main_project;

    if (defined $opt_work_dir) {
        # Now scan the directory tree looking for source files and, maybe, targets
        print "Scanning the source directories...\n";
        source_scan_directory(\@main_project,"","",0);
        @projects=sort { @$a[$P_PATH] cmp @$b[$P_PATH] } @projects;
    } elsif (defined $opt_work_file) {
        if ($opt_work_file =~ /.dsp$/i or $opt_work_file =~ /.vcproj$/i) {
            source_scan_project_file(\@main_project,0,$opt_work_file);
        } elsif ($opt_work_file =~ /.dsw$/i or $opt_work_file =~ /.sln$/i) {
            source_scan_workspace_file($opt_work_file);
        }
    }
}

#####
#
# Source search
#
#####

##
# Performs a directory traversal and renames the files so that:
# - they have the case desired by the user
# - their extension is of the appropriate case
# - they don't contain annoying characters like ' ', '$', '#', ...
# But only perform these changes for source files and directories.
sub fix_file_and_directory_names($);
sub fix_file_and_directory_names($)
{
  my $dirname=$_[0];

  my $directory=get_directory_contents($dirname);
  foreach my $dentry (@$directory)
  {
      if ($dentry =~ /^\./ or $dentry eq "CVS") {
          next;
      }
      # Set $warn to 1 if the user should be warned of the renaming
      my $warn;
      my $new_name=$dentry;

      if (-f "$dirname/$dentry")
      {
          # Don't rename Winemaker's makefiles
          next if ($dentry eq "Makefile" and
                   `head -n 1 "$dirname/$dentry"` =~ /Generated by Winemaker/);

          # Leave non-source files alone
          next if ($new_name !~ /(^makefile|\.(c|cpp|h|rc|spec|def))$/i);

          # Only all lowercase extensions are supported (because of
          # rules like '.c.o:').
          $new_name =~ s/\.C$/.c/;
          $new_name =~ s/\.cpp$/.cpp/i;
          $warn=1 if ($new_name =~ s/\.cxx$/.cpp/i);
          $new_name =~ s/\.rc$/.rc/i;
          # And this last one is to avoid confusion when running make
          $warn=1 if ($new_name =~ s/^makefile$/makefile.win/i);
      }

      # Adjust the case to the user's preferences
      if (($opt_lower == $OPT_LOWER_ALL and $dentry =~ /[A-Z]/) or
          ($opt_lower == $OPT_LOWER_UPPERCASE and $dentry !~ /[a-z]/)
         ) {
          $new_name=lc $new_name;
      }

      # make doesn't support these characters well
      $new_name =~ s/[ \$]/_/g;

      # And finally, perform the renaming
      if ($new_name ne $dentry)
      {
          if ($warn) {
              print STDERR "warning: in \"$dirname\", renaming \"$dentry\" to \"$new_name\"\n";
          }
          if (!rename("$dirname/$dentry","$dirname/$new_name")) {
              print STDERR "error: in \"$dirname\", unable to rename \"$dentry\" to \"$new_name\"\n";
              print STDERR "       $!\n";
              $new_name=$dentry;
          }
          else
          {
              clear_directory_cache($dirname);
          }
      }
      if (-d "$dirname/$new_name") {
          fix_file_and_directory_names("$dirname/$new_name");
      }
  }
}



#####
#
# Source fixup
#
#####

##
# Try to find a file for the specified filename. The attempt is
# case-insensitive which is why it's not trivial. If a match is
# found then we return the pathname with the correct case.
sub search_from($$)
{
  my $dirname=$_[0];
  my $path=$_[1];
  my $real_path="";

  if ($dirname eq "" or $dirname eq "." or $dirname eq "./") {
    $dirname=cwd;
  } elsif ($dirname !~ m+^/+) {
    $dirname=cwd . "/" . $dirname;
  }
  if ($dirname !~ m+/$+) {
    $dirname.="/";
  }

  foreach my $component (@$path) {
    $component=~s/^\"//;
    $component=~s/\"$//;
    #print "    looking for $component in \"$dirname\"\n";
    if ($component eq ".") {
      # Pass it as is
      $real_path.="./";
    } elsif ($component eq "..") {
      # Go up one level
      if ($dirname =~ /\.\.\/$/) {
        $dirname.="../";
      } else {
        $dirname=dirname($dirname) . "/";
      }
      $real_path.="../";
    } else {
      # The file/directory may have been renamed before. Also try to
      # match the renamed file.
      my $renamed=$component;
      $renamed =~ s/[ \$]/_/g;
      if ($renamed eq $component) {
        undef $renamed;
      }

      my $directory=get_directory_contents $dirname;
      my $found;
      foreach my $dentry (@$directory) {
	if ($dentry =~ /^\Q$component\E$/i or
            (defined $renamed and $dentry =~ /^$renamed$/i)
           ) {
	  $dirname.="$dentry/";
	  $real_path.="$dentry/";
	  $found=1;
	  last;
	}
      }
      if (!defined $found) {
	# Give up
	#print "    could not find $component in $dirname\n";
	return;
      }
    }
  }
  $real_path=~ s+/$++;
  #print "    -> found $real_path\n";
  return $real_path;
}

##
# Performs a case-insensitive search for the specified file in the
# include path.
# $line is the line number that should be referenced when an error occurs
# $filename is the file we are looking for
# $dirname is the directory of the file containing the '#include' directive
#    if '"' was used, it is an empty string otherwise
# $project and $target specify part of the include path
sub get_real_include_name($$$$$)
{
  my $line=$_[0];
  my $filename=$_[1];
  my $dirname=$_[2];
  my $project=$_[3];
  my $target=$_[4];

  if ($filename =~ /^([a-zA-Z]:)?[\/\\]/ or $filename =~ /^[a-zA-Z]:[\/\\]?/) {
    # This is not a relative path, we cannot make any check
    my $warning="path:$filename";
    if (!defined $warnings{$warning}) {
      $warnings{$warning}="1";
      print STDERR "warning: cannot check the case of absolute pathnames:\n";
      print STDERR "$line:   $filename\n";
    }
  } else {
    # Here's how we proceed:
    # - split the filename we look for into its components
    # - then for each directory in the include path
    #   - trace the directory components starting from that directory
    #   - if we fail to find a match at any point then continue with
    #     the next directory in the include path
    #   - otherwise, rejoice, our quest is over.
    my @file_components=split /[\/\\]+/, $filename;
    #print "  Searching for $filename from @$project[$P_PATH]\n";

    my $real_filename;
    if ($dirname ne "") {
      # This is an 'include ""' -> look in dirname first.
      #print "    in $dirname (include \"\")\n";
      $real_filename=search_from($dirname,\@file_components);
      if (defined $real_filename) {
	return $real_filename;
      }
    }
    my $project_settings=@$project[$P_SETTINGS];
    foreach my $include (@{@$target[$T_INCLUDE_PATH]}, @{@$project_settings[$T_INCLUDE_PATH]}) {
      my $dirname=$include;
      $dirname=~ s+^-I++;
      $dirname=~ s+\s$++;
      if (!is_absolute($dirname)) {
	$dirname="@$project[$P_PATH]$dirname";
      } else {
        $dirname=~ s+^\$\(TOPSRCDIR\)/++;
        $dirname=~ s+^\$\(SRCDIR\)/+@$project[$P_PATH]+;
      }
      #print "    in $dirname\n";
      $real_filename=search_from("$dirname",\@file_components);
      if (defined $real_filename) {
	return $real_filename;
      }
    }
    my $dotdotpath=@$project[$P_PATH];
    $dotdotpath =~ s/[^\/]+/../g;
    foreach my $include (@{$global_settings[$T_INCLUDE_PATH]}) {
      my $dirname=$include;
      $dirname=~ s+^-I++;
      $dirname=~ s+^\$\(TOPSRCDIR\)\/++;
      $dirname=~ s+^\$\(SRCDIR\)\/+@$project[$P_PATH]+;
      #print "    in $dirname  (global setting)\n";
      $real_filename=search_from("$dirname",\@file_components);
      if (defined $real_filename) {
	return $real_filename;
      }
    }
  }
  $filename =~ s+\\\\+/+g; # in include ""
  $filename =~ s+\\+/+g; # in include <> !
  if ($opt_lower_include) {
    return lc "$filename";
  }
  return $filename;
}

sub print_pack($$$)
{
  my $indent=$_[0];
  my $size=$_[1];
  my $trailer=$_[2];

  if ($size =~ /^(1|2|4|8)$/) {
    print FILEO "$indent#include <pshpack$size.h>$trailer";
  } else {
    print FILEO "$indent/* winemaker:warning: Unknown size \"$size\". Defaulting to 4 */\n";
    print FILEO "$indent#include <pshpack4.h>$trailer";
  }
}

##
# 'Parses' a source file and fixes constructs that would not work with
# Winelib. The parsing is rather simple and not all non-portable features
# are corrected. The most important feature that is corrected is the case
# and path separator of '#include' directives. This requires that each
# source file be associated to a project & target so that the proper
# include path is used.
# Also note that the include path is relative to the directory in which the
# compiler is run, i.e. that of the project, not to that of the file.
sub fix_file($$$)
{
  my $filename=$_[0];
  my $project=$_[1];
  my $target=$_[2];
  $filename="@$project[$P_PATH]$filename";
  if (! -e $filename) {
    return;
  }

  my $is_rc=($filename =~ /\.(rc2?|dlg)$/i);
  my $dirname=dirname($filename);
  my $is_mfc=0;
  if (defined $target and (@$target[$T_FLAGS] & $TF_MFC)) {
    $is_mfc=1;
  }

  print "  $filename\n";
  if (! -e "$filename.bak") {
    if (!copy("$filename","$filename.bak")) {
      print STDERR "error: unable to make a backup of $filename:\n";
      print STDERR "       $!\n";
      return;
    }
  }
  if (!open(FILEI,"$filename.bak")) {
    print STDERR "error: unable to open $filename.bak for reading:\n";
    print STDERR "       $!\n";
    return;
  }
  if (!open(FILEO,">$filename")) {
    print STDERR "error: unable to open $filename for writing:\n";
    print STDERR "       $!\n";
    return;
  }
  my $line=0;
  my $modified=0;
  my $rc_block_depth=0;
  my $rc_textinclude_state=0;
  my @pack_stack;
  while (<FILEI>) {
    # Remove any trailing CtrlZ, which isn't strictly in the file
    if (/\x1A/) {
      s/\x1A//;
      last if (/^$/)
    }
    $line++;
    s/\r\n$/\n/;
    if (!/\n$/) {
      # Make sure all files are '\n' terminated
      $_ .= "\n";
    }
    if ($is_rc and !$is_mfc and /^(\s*)(\#\s*include\s*)\"afxres\.h\"/) {
      # VC6 automatically includes 'afxres.h', an MFC specific header, in
      # the RC files it generates (even in non-MFC projects). So we replace
      # it with 'winresrc.h' its very close standard cousin so that non MFC
      # projects can compile in Wine without the MFC sources.
      my $warning="mfc:afxres.h";
      if (!defined $warnings{$warning}) {
	$warnings{$warning}="1";
	print STDERR "warning: In non-MFC projects, winemaker replaces the MFC specific header 'afxres.h' with 'winresrc.h'\n";
	print STDERR "warning: the above warning is issued only once\n";
      }
      print FILEO "$1/* winemaker: $2\"afxres.h\" */\n";
      print FILEO "$1/* winemaker:warning: 'afxres.h' is an MFC specific header. Replacing it with 'winresrc.h' */\n";
      print FILEO "$1$2\"winresrc.h\"$'";
      $modified=1;

    } elsif (/^(\s*\#\s*include\s*)([\"<])([^\"]+)([\">])/) {
      my $from_file=($2 eq "<"?"":$dirname);
      my $real_include_name=get_real_include_name($line,$3,$from_file,$project,$target);
      print FILEO "$1$2$real_include_name$4$'";
      $modified|=($real_include_name ne $3);

    } elsif (/^(\s*)\#\s*pragma\s+comment\s*\(\s*lib\s*,\s*\"(\w+)\.lib\"\s*\)/) {
      my $pragma_indent=$1;
      my $pragma_lib=$2;
      push @{@$target[$T_LIBRARIES]},$pragma_lib;
      print FILEO "$pragma_indent/* winemaker: Added -l$pragma_lib to the libraries */\n";
    } elsif (s/^(\s*)(\#\s*pragma\s+pack\s*\(\s*)//) {
      # Pragma pack handling
      #
      # pack_stack is an array of references describing the stack of
      # pack directives currently in effect. Each directive if described
      # by a reference to an array containing:
      # - "push" for pack(push,...) directives, "" otherwise
      # - the directive's identifier at index 1
      # - the directive's alignment value at index 2
      #
      # Don't believe a word of what the documentation says: it's all wrong.
      # The code below is based on the actual behavior of Visual C/C++ 6.
      my $pack_indent=$1;
      my $pack_header=$2;
      if (/^(\))/) {
        # pragma pack()
        # Pushes the default stack alignment
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        print FILEO "$pack_indent/* winemaker:warning: Using 4 as the default alignment */\n";
        print_pack($pack_indent,4,$');
        push @pack_stack, [ "", "", 4 ];

      } elsif (/^(pop\s*(,\s*\d+\s*)?\))/) {
        # pragma pack(pop)
        # pragma pack(pop,n)
        # Goes up the stack until it finds a pack(push,...), and pops it
        # Ignores any pack(n) entry
        # Issues a warning if the pack is of the form pack(push,label)
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        my $pack_comment=$';
        $pack_comment =~ s/^\s*//;
        if ($pack_comment ne "") {
          print FILEO "$pack_indent$pack_comment";
        }
        while (1) {
          my $alignment=pop @pack_stack;
          if (!defined $alignment) {
            print FILEO "$pack_indent/* winemaker:warning: No pack(push,...) found. All the stack has been popped */\n";
            last;
          }
          if (@$alignment[1]) {
            print FILEO "$pack_indent/* winemaker:warning: Anonymous pop of pack(push,@$alignment[1]) (@$alignment[2]) */\n";
          }
          print FILEO "$pack_indent#include <poppack.h>\n";
          if (@$alignment[0]) {
            last;
          }
        }

      } elsif (/^(pop\s*,\s*(\w+)\s*(,\s*\d+\s*)?\))/) {
        # pragma pack(pop,label[,n])
        # Goes up the stack until finding a pack(push,...) and pops it.
        # 'n', if specified, is ignored.
        # Ignores any pack(n) entry
        # Issues a warning if the label of the pack does not match,
        # or if it is in fact a pack(push,n)
        my $label=$2;
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        my $pack_comment=$';
        $pack_comment =~ s/^\s*//;
        if ($pack_comment ne "") {
          print FILEO "$pack_indent$pack_comment";
        }
        while (1) {
          my $alignment=pop @pack_stack;
          if (!defined $alignment) {
            print FILEO "$pack_indent/* winemaker:warning: No pack(push,$label) found. All the stack has been popped */\n";
            last;
          }
          if (@$alignment[1] and @$alignment[1] ne $label) {
            print FILEO "$pack_indent/* winemaker:warning: Push/pop mismatch: \"@$alignment[1]\" (@$alignment[2]) != \"$label\" */\n";
          }
          print FILEO "$pack_indent#include <poppack.h>\n";
          if (@$alignment[0]) {
            last;
          }
        }

      } elsif (/^(push\s*\))/) {
        # pragma pack(push)
        # Push the current alignment
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        if (@pack_stack > 0) {
          my $alignment=$pack_stack[$#pack_stack];
          print_pack($pack_indent,@$alignment[2],$');
          push @pack_stack, [ "push", "", @$alignment[2] ];
        } else {
          print FILEO "$pack_indent/* winemaker:warning: Using 4 as the default alignment */\n";
          print_pack($pack_indent,4,$');
          push @pack_stack, [ "push", "", 4 ];
        }

      } elsif (/^((push\s*,\s*)?(\d+)\s*\))/) {
        # pragma pack([push,]n)
        # Push new alignment n
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        print_pack($pack_indent,$3,"$'");
        push @pack_stack, [ ($2 ? "push" : ""), "", $3 ];

      } elsif (/^((\w+)\s*\))/) {
        # pragma pack(label)
        # label must in fact be a macro that resolves to an integer
        # Then behaves like 'pragma pack(n)'
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        print FILEO "$pack_indent/* winemaker:warning: Assuming $2 == 4 */\n";
        print_pack($pack_indent,4,$');
        push @pack_stack, [ "", "", 4 ];

      } elsif (/^(push\s*,\s*(\w+)\s*(,\s*(\d+)\s*)?\))/) {
        # pragma pack(push,label[,n])
        # Pushes a new label on the stack. It is possible to push the same
        # label multiple times. If 'n' is omitted then the alignment is
        # unchanged. Otherwise it becomes 'n'.
        print FILEO "$pack_indent/* winemaker: $pack_header$1 */\n";
        my $size;
        if (defined $4) {
          $size=$4;
        } elsif (@pack_stack > 0) {
          my $alignment=$pack_stack[$#pack_stack];
          $size=@$alignment[2];
        } else {
          print FILEO "$pack_indent/* winemaker:warning: Using 4 as the default alignment */\n";
          $size=4;
        }
        print_pack($pack_indent,$size,$');
        push @pack_stack, [ "push", $2, $size ];

      } else {
        # pragma pack(???               -> What's that?
        print FILEO "$pack_indent/* winemaker:warning: Unknown type of pragma pack directive */\n";
        print FILEO "$pack_indent$pack_header$_";

      }
      $modified=1;

    } elsif ($is_rc) {
      if ($rc_block_depth == 0 and /^(\w+\s+(BITMAP|CURSOR|FONT|FONTDIR|ICON|MESSAGETABLE|TEXT|RTF)\s+((DISCARDABLE|FIXED|IMPURE|LOADONCALL|MOVEABLE|PRELOAD|PURE)\s+)*)([\"<]?)([^\">\r\n]+)([\">]?)/) {
	my $from_file=($5 eq "<"?"":$dirname);
	my $real_include_name=get_real_include_name($line,$6,$from_file,$project,$target);
	print FILEO "$1$5$real_include_name$7$'";
	$modified|=($real_include_name ne $6);

      } elsif (/^(\s*RCINCLUDE\s*)([\"<]?)([^\">\r\n]+)([\">]?)/) {
	my $from_file=($2 eq "<"?"":$dirname);
	my $real_include_name=get_real_include_name($line,$3,$from_file,$project,$target);
	print FILEO "$1$2$real_include_name$4$'";
	$modified|=($real_include_name ne $3);

      } elsif ($is_rc and !$is_mfc and $rc_block_depth == 0 and /^\s*\d+\s+TEXTINCLUDE\s*/) {
	$rc_textinclude_state=1;
	print FILEO;

      } elsif ($rc_textinclude_state == 3 and /^(\s*\"\#\s*include\s*\"\")afxres\.h(\"\"\\r\\n\")/) {
	print FILEO "$1winresrc.h$2$'";
	$modified=1;

      } elsif (/^\s*BEGIN(\W.*)?$/) {
	$rc_textinclude_state|=2;
	$rc_block_depth++;
	print FILEO;

      } elsif (/^\s*END(\W.*)?$/) {
	$rc_textinclude_state=0;
	if ($rc_block_depth>0) {
	  $rc_block_depth--;
	}
	print FILEO;

      } else {
	print FILEO;
      }

    } else {
      print FILEO;
    }
  }

  close(FILEI);
  close(FILEO);
  if ($opt_backup == 0 or $modified == 0) {
    if (!unlink("$filename.bak")) {
      print STDERR "error: unable to delete $filename.bak:\n";
      print STDERR "       $!\n";
    }
  }
}

##
# Analyzes each source file in turn to find and correct issues
# that would cause it not to compile.
sub fix_source()
{
  print "Fixing the source files...\n";
  foreach my $project (@projects) {
    foreach my $target (@$project[$P_SETTINGS],@{@$project[$P_TARGETS]}) {
      foreach my $source (@{@$target[$T_SOURCES_C]}, @{@$target[$T_SOURCES_CXX]}, @{@$target[$T_SOURCES_RC]}, @{@$target[$T_SOURCES_MISC]}) {
	fix_file($source,$project,$target);
      }
    }
  }
}



#####
#
# File generation
#
#####

##
# A convenience function to generate all the lists (defines,
# C sources, C++ source, etc.) in the Makefile
sub generate_list($$$;$)
{
  my $name=$_[0];
  my $last=$_[1];
  my $list=$_[2];
  my $data=$_[3];
  my $first=$name;

  if ($name) {
    printf FILEO "%-22s=",$name;
  }
  if (defined $list) {
    foreach my $item (@$list) {
      my $value;
      if (defined $data) {
        $value=&$data($item);
      } else {
        if (defined $item) {
          $value=$item;
        } else {
          $value="";
        }
      }
      if ($value ne "") {
	if ($first) {
	  print FILEO " $value";
	  $first=0;
	} else {
	  print FILEO " \\\n\t\t\t$value";
	}
      }
    }
  }
  if ($last) {
    print FILEO "\n";
  }
}

##
# Generates a project's Makefile and all the target files
sub generate_project_files($)
{
  my $project=$_[0];
  my $project_settings=@$project[$P_SETTINGS];
  my @dll_list=();
  my @lib_list=();
  my @exe_list=();

  # Then sort the targets and separate the libraries from the programs
  foreach my $target (sort { @$a[$T_NAME] cmp @$b[$T_NAME] } @{@$project[$P_TARGETS]}) {
    if (@$target[$T_TYPE] == $TT_DLL) {
      push @dll_list,$target;
    } elsif (@$target[$T_TYPE] == $TT_LIB) {
      push @lib_list,$target;
    } else {
      push @exe_list,$target;
    }
  }
  @$project[$P_TARGETS]=[];
  push @{@$project[$P_TARGETS]}, @dll_list;
  push @{@$project[$P_TARGETS]}, @lib_list;
  push @{@$project[$P_TARGETS]}, @exe_list;

  if (!open(FILEO,">@$project[$P_PATH]Makefile")) {
    print STDERR "error: could not open \"@$project[$P_PATH]/Makefile\" for writing\n";
    print STDERR "       $!\n";
    return;
  }
  binmode( FILEO, ':utf8' );

  my $cpp_to_object;
  if (@$project_settings[$T_FLAGS] & $TF_HASCXX) {
    $cpp_to_object=".cxx=.o";
  } else {
    $cpp_to_object=".cpp=.o";
  }

  print FILEO "### Generated by Winemaker $version\n";
  print FILEO "###\n";
  print FILEO "### Invocation command line was\n";
  print FILEO "### $0";
  foreach(@ARGV) {
    print FILEO " $_";
  }
  print FILEO "\n\n\n";

  generate_list("SRCDIR",1,[ "." ]);
  if (@$project[$P_PATH] eq "") {
    # This is the main project. It is also responsible for recursively
    # calling the other projects
    generate_list("SUBDIRS",1,\@projects,sub
		  {
		    if ($_[0] != \@main_project) {
		      my $subdir=@{$_[0]}[$P_PATH];
		      $subdir =~ s+/$++;
		      return $subdir;
		    }
		    # Eliminating the main project by returning undefined!
		  });
  }
  if (@{@$project[$P_TARGETS]} > 0) {
    generate_list("DLLS",1,\@dll_list,sub
		  {
		    return @{$_[0]}[$T_NAME];
		  });
    generate_list("LIBS",1,\@lib_list,sub
		  {
		    return @{$_[0]}[$T_NAME];
		  });
    generate_list("EXES",1,\@exe_list,sub
		  {
		    return "@{$_[0]}[$T_NAME]";
		  });
    print FILEO "\n\n\n";

    print FILEO "### Common settings\n\n";
    # Make it so that the project-wide settings override the global settings
    generate_list("CEXTRA",1,@$project_settings[$T_CEXTRA]);
    generate_list("CXXEXTRA",1,@$project_settings[$T_CXXEXTRA]);
    generate_list("RCEXTRA",1,@$project_settings[$T_RCEXTRA]);
    generate_list("DEFINES",1,@$project_settings[$T_DEFINES]);
    generate_list("INCLUDE_PATH",1,@$project_settings[$T_INCLUDE_PATH]);
    generate_list("DLL_PATH",1,@$project_settings[$T_DLL_PATH]);
    generate_list("DLL_IMPORTS",1,@$project_settings[$T_DLLS]);
    generate_list("LIBRARY_PATH",1,@$project_settings[$T_LIBRARY_PATH]);
    generate_list("LIBRARIES",1,@$project_settings[$T_LIBRARIES]);
    print FILEO "\n\n";

    my $extra_source_count=@{@$project_settings[$T_SOURCES_C]}+
                           @{@$project_settings[$T_SOURCES_CXX]}+
                           @{@$project_settings[$T_SOURCES_RC]};
    my $no_extra=($extra_source_count == 0);
    if (!$no_extra) {
      print FILEO "### Extra source lists\n\n";
      generate_list("EXTRA_C_SRCS",1,@$project_settings[$T_SOURCES_C]);
      generate_list("EXTRA_CXX_SRCS",1,@$project_settings[$T_SOURCES_CXX]);
      generate_list("EXTRA_RC_SRCS",1,@$project_settings[$T_SOURCES_RC]);
      print FILEO "\n";
      generate_list("EXTRA_OBJS",1,["\$(EXTRA_C_SRCS:.c=.o)","\$(EXTRA_CXX_SRCS:$cpp_to_object)"]);
      print FILEO "\n\n\n";
    }

    # Iterate over all the targets...
    foreach my $target (@{@$project[$P_TARGETS]}) {
      print FILEO "### @$target[$T_NAME] sources and settings\n\n";
      my $canon=canonize("@$target[$T_NAME]");
      $canon =~ s+_so$++;

      generate_list("${canon}_MODULE",1,[@$target[$T_NAME]]);
      generate_list("${canon}_C_SRCS",1,@$target[$T_SOURCES_C]);
      generate_list("${canon}_CXX_SRCS",1,@$target[$T_SOURCES_CXX]);
      generate_list("${canon}_RC_SRCS",1,@$target[$T_SOURCES_RC]);
      generate_list("${canon}_LDFLAGS",1,@$target[$T_LDFLAGS]);
      generate_list("${canon}_ARFLAGS",1,@$target[$T_ARFLAGS]);
      generate_list("${canon}_DLL_PATH",1,@$target[$T_DLL_PATH]);
      generate_list("${canon}_DLLS",1,@$target[$T_DLLS]);
      generate_list("${canon}_LIBRARY_PATH",1,@$target[$T_LIBRARY_PATH]);
      generate_list("${canon}_LIBRARIES",1,@$target[$T_LIBRARIES]);
      print FILEO "\n";
      generate_list("${canon}_OBJS",1,["\$(${canon}_C_SRCS:.c=.o)","\$(${canon}_CXX_SRCS:$cpp_to_object)","\$(${canon}_RC_SRCS:.rc=.res)"]);
      print FILEO "\n\n\n";
    }
    print FILEO "### Global source lists\n\n";
    generate_list("C_SRCS",$no_extra,@$project[$P_TARGETS],sub
		  {
		    my $canon=canonize(@{$_[0]}[$T_NAME]);
		    $canon =~ s+_so$++;
		    return "\$(${canon}_C_SRCS)";
		  });
    if (!$no_extra) {
      generate_list("",1,[ "\$(EXTRA_C_SRCS)" ]);
    }
    generate_list("CXX_SRCS",$no_extra,@$project[$P_TARGETS],sub
		  {
		    my $canon=canonize(@{$_[0]}[$T_NAME]);
		    $canon =~ s+_so$++;
		    return "\$(${canon}_CXX_SRCS)";
		  });
    if (!$no_extra) {
      generate_list("",1,[ "\$(EXTRA_CXX_SRCS)" ]);
    }
    generate_list("RC_SRCS",$no_extra,@$project[$P_TARGETS],sub
		  {
		    my $canon=canonize(@{$_[0]}[$T_NAME]);
		    $canon =~ s+_so$++;
		    return "\$(${canon}_RC_SRCS)";
		  });
    if (!$no_extra) {
      generate_list("",1,[ "\$(EXTRA_RC_SRCS)" ]);
    }
  }
  print FILEO "\n\n";
  print FILEO "### Tools\n\n";
  print FILEO "CC = winegcc\n";
  print FILEO "CXX = wineg++\n";
  print FILEO "RC = wrc\n";
  print FILEO "AR = ar\n";
  print FILEO "\n\n";

  print FILEO "### Generic targets\n\n";
  print FILEO "all:";
  if (@$project[$P_PATH] eq "") {
    print FILEO " \$(SUBDIRS)";
  }
  if (@{@$project[$P_TARGETS]} > 0) {
    print FILEO " \$(DLLS:%=%.so) \$(LIBS) \$(EXES)";
  }
  print FILEO "\n\n";
  print FILEO "### Build rules\n";
  print FILEO "\n";
  print FILEO ".PHONY: all clean dummy\n";
  print FILEO "\n";
  print FILEO "\$(SUBDIRS): dummy\n";
  print FILEO "\t\@cd \$\@ && \$(MAKE)\n";
  print FILEO "\n";
  print FILEO "# Implicit rules\n";
  print FILEO "\n";
  print FILEO ".SUFFIXES: .cpp .cxx .rc .res\n";
  print FILEO "DEFINCL = \$(INCLUDE_PATH) \$(DEFINES) \$(OPTIONS)\n";
  print FILEO "\n";
  print FILEO ".c.o:\n";
  print FILEO "\t\$(CC) -c \$(CFLAGS) \$(CEXTRA) \$(DEFINCL) -o \$\@ \$<\n";
  print FILEO "\n";
  print FILEO ".cpp.o:\n";
  print FILEO "\t\$(CXX) -c \$(CXXFLAGS) \$(CXXEXTRA) \$(DEFINCL) -o \$\@ \$<\n";
  print FILEO "\n";
  print FILEO ".cxx.o:\n";
  print FILEO "\t\$(CXX) -c \$(CXXFLAGS) \$(CXXEXTRA) \$(DEFINCL) -o \$\@ \$<\n";
  print FILEO "\n";
  print FILEO ".rc.res:\n";
  print FILEO "\t\$(RC) \$(RCFLAGS) \$(RCEXTRA) \$(DEFINCL) -fo\$@ \$<\n";
  print FILEO "\n";
  print FILEO "# Rules for cleaning\n";
  print FILEO "\n";
  print FILEO "CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \\\n";
  print FILEO "                  \\\\\\#*\\\\\\# *~ *% .\\\\\\#*\n";
  print FILEO "\n";
  print FILEO "clean:: \$(SUBDIRS:%=%/__clean__) \$(EXTRASUBDIRS:%=%/__clean__)\n";
  print FILEO "\t\$(RM) \$(CLEAN_FILES) \$(RC_SRCS:.rc=.res) \$(C_SRCS:.c=.o) \$(CXX_SRCS:$cpp_to_object)\n";
  print FILEO "\t\$(RM) \$(DLLS:%=%.so) \$(LIBS) \$(EXES) \$(EXES:%=%.so)\n";
  print FILEO "\n";
  print FILEO "\$(SUBDIRS:%=%/__clean__): dummy\n";
  print FILEO "\tcd `dirname \$\@` && \$(MAKE) clean\n";
  print FILEO "\n";
  print FILEO "\$(EXTRASUBDIRS:%=%/__clean__): dummy\n";
  print FILEO "\t-cd `dirname \$\@` && \$(RM) \$(CLEAN_FILES)\n";
  print FILEO "\n";

  if (@{@$project[$P_TARGETS]} > 0) {
    print FILEO "### Target specific build rules\n";
    print FILEO "DEFLIB = \$(LIBRARY_PATH) \$(LIBRARIES) \$(DLL_PATH) \$(DLL_IMPORTS:%=-l%)\n\n";
    foreach my $target (@{@$project[$P_TARGETS]}) {
      my $canon=canonize("@$target[$T_NAME]");
      $canon =~ s/_so$//;

      if (@$target[$T_TYPE] == $TT_DLL && (@$project_settings[$T_FLAGS] & $TF_HASDEF)) {
        print FILEO "\$(${canon}_MODULE).so: \$(${canon}_OBJS) \$(${canon}_MODULE:.dll=.def)\n";
      } elsif (@$target[$T_TYPE] == $TT_DLL) {
        print FILEO "\$(${canon}_MODULE).so: \$(${canon}_OBJS) \$(${canon}_MODULE:.dll=.spec)\n";
      } else {
        print FILEO "\$(${canon}_MODULE): \$(${canon}_OBJS)\n";
      }

      if (@$target[$T_TYPE] == $TT_LIB) {
        print FILEO "\t\$(AR) \$(${canon}_ARFLAGS) \$\@ \$(${canon}_OBJS)\n";
      } else {
        if (@{@$target[$T_SOURCES_CXX]} > 0 or @{@$project_settings[$T_SOURCES_CXX]} > 0) {
          print FILEO "\t\$(CXX)";
        } else {
          print FILEO "\t\$(CC)";
        }
        print FILEO " \$(${canon}_LDFLAGS) -o \$\@ \$(${canon}_OBJS) \$(${canon}_LIBRARY_PATH) \$(${canon}_DLL_PATH) \$(DEFLIB) \$(${canon}_DLLS:%=-l%) \$(${canon}_LIBRARIES:%=-l%)\n";
      }
      print FILEO "\n\n";
    }
  }
  close(FILEO);

}


##
# This is where we finally generate files. In fact this method does not
# do anything itself but calls the methods that do the actual work.
sub generate()
{
  print "Generating project files...\n";

  foreach my $project (@projects) {
    my $path=@$project[$P_PATH];
    if ($path eq "") {
      $path=".";
    } else {
      $path =~ s+/$++;
    }
    print "  $path\n";
    generate_project_files($project);
  }
}



#####
#
# Option defaults
#
#####

$opt_backup=1;
$opt_lower=$OPT_LOWER_UPPERCASE;
$opt_lower_include=1;

$opt_work_dir=undef;
$opt_single_target=undef;
$opt_target_type=$TT_GUIEXE;
$opt_flags=0;
$opt_arch=$OPT_ARCH_DEFAULT;
$opt_is_interactive=$OPT_ASK_NO;
$opt_ask_project_options=$OPT_ASK_NO;
$opt_ask_target_options=$OPT_ASK_NO;
$opt_no_generated_files=0;
$opt_no_source_fix=0;
$opt_no_banner=0;



#####
#
# Main
#
#####

sub print_banner()
{
  print "Winemaker $version\n";
  print "Copyright 2000-2004 Fran�ois Gouget <fgouget\@codeweavers.com> for CodeWeavers\n";
  print "Copyright 2004 Dimitrie O. Paun\n";
  print "Copyright 2009-2012 Andr� Hentschel\n";
}

sub usage()
{
  print_banner();
  print STDERR "Usage: winemaker [--nobanner] [--backup|--nobackup] [--nosource-fix]\n";
  print STDERR "                 [--lower-none|--lower-all|--lower-uppercase]\n";
  print STDERR "                 [--lower-include|--nolower-include] [--mfc|--nomfc]\n";
  print STDERR "                 [--guiexe|--windows|--cuiexe|--console|--dll|--lib]\n";
  print STDERR "                 [-Dmacro[=defn]] [-Idir] [-Pdir] [-idll] [-Ldir] [-llibrary]\n";
  print STDERR "                 [--nodlls] [--nomsvcrt] [--interactive] [--single-target name]\n";
  print STDERR "                 [--generated-files|--nogenerated-files]\n";
  print STDERR "                 [--wine32]\n";
  print STDERR "                 work_directory|project_file|workspace_file\n";
  print STDERR "\nWinemaker is designed to recursively convert all the Windows sources found in\n";
  print STDERR "the specified directory or project-file, so that they can be compiled with Winelib.\n";
  print STDERR "During this process it will modify and rename some of the corresponding files.\n";
  print STDERR "\tPlease read the manual page before use.\n";
  exit (2);
}

binmode(STDOUT, ":utf8");

target_init(\@global_settings);

foreach(@ARGV) {
  my $arg=$_;
  # General options
  if ($arg eq "--nobanner") {
    $opt_no_banner=1;
  } elsif ($arg eq "--backup") {
    $opt_backup=1;
  } elsif ($arg eq "--nobackup") {
    $opt_backup=0;
  } elsif ($arg eq "--single-target") {
    $opt_single_target=shift @ARGV;
  } elsif ($arg eq "--lower-none") {
    $opt_lower=$OPT_LOWER_NONE;
  } elsif ($arg eq "--lower-all") {
    $opt_lower=$OPT_LOWER_ALL;
  } elsif ($arg eq "--lower-uppercase") {
    $opt_lower=$OPT_LOWER_UPPERCASE;
  } elsif ($arg eq "--lower-include") {
    $opt_lower_include=1;
  } elsif ($arg eq "--nolower-include") {
    $opt_lower_include=0;
  } elsif ($arg eq "--nosource-fix") {
    $opt_no_source_fix=1;
  } elsif ($arg eq "--generated-files") {
    $opt_no_generated_files=0;
  } elsif ($arg eq "--nogenerated-files") {
    $opt_no_generated_files=1;
  } elsif ($arg eq "--wine32") {
    $opt_arch=$OPT_ARCH_32;
  } elsif ($arg =~ /^-D/) {
    push @{$global_settings[$T_DEFINES]},$arg;
  } elsif ($arg =~ /^-I/) {
    push @{$global_settings[$T_INCLUDE_PATH]},$arg;
  } elsif ($arg =~ /^-P/) {
    push @{$global_settings[$T_DLL_PATH]},"-L$'";
  } elsif ($arg =~ /^-i/) {
    push @{$global_settings[$T_DLLS]},$';
  } elsif ($arg =~ /^-L/) {
    push @{$global_settings[$T_LIBRARY_PATH]},$arg;
  } elsif ($arg =~ /^-l/) {
    push @{$global_settings[$T_LIBRARIES]},$arg;

  # 'Source'-based method options
  } elsif ($arg eq "--dll") {
    $opt_target_type=$TT_DLL;
  } elsif ($arg eq "--lib") {
    $opt_target_type=$TT_LIB;
  } elsif ($arg eq "--guiexe" or $arg eq "--windows") {
    $opt_target_type=$TT_GUIEXE;
  } elsif ($arg eq "--cuiexe" or $arg eq "--console") {
    $opt_target_type=$TT_CUIEXE;
  } elsif ($arg eq "--interactive") {
    $opt_is_interactive=$OPT_ASK_YES;
    $opt_ask_project_options=$OPT_ASK_YES;
    $opt_ask_target_options=$OPT_ASK_YES;
  } elsif ($arg eq "--mfc") {
    $opt_flags|=$TF_MFC;
  } elsif ($arg eq "--nomfc") {
    $opt_flags&=~$TF_MFC;
    $opt_flags|=$TF_NOMFC;
  } elsif ($arg eq "--nodlls") {
    $opt_flags|=$TF_NODLLS;
  } elsif ($arg eq "--nomsvcrt") {
    $opt_flags|=$TF_NOMSVCRT;

  # Catch errors
  } else {
    if ($arg ne "--help" and $arg ne "-h" and $arg ne "-?") {
        if (!defined $opt_work_dir and !defined $opt_work_file) {
            if (-f $arg) {
                $opt_work_file=$arg;
            }
            else {
                $opt_work_dir=$arg;
            }
        } else {
            print STDERR "error: the work directory, \"$arg\", has already been specified (was \"$opt_work_dir\")\n";
            usage();
        }
    } else {
        usage();
    }
  }
}

if (!defined $opt_work_dir and !defined $opt_work_file) {
  print STDERR "error: you must specify the directory or project file containing the sources to be converted\n";
  usage();
} elsif (defined $opt_work_dir and !chdir $opt_work_dir) {
  print STDERR "error: could not chdir to the work directory\n";
  print STDERR "       $!\n";
  usage();
}

if ($opt_no_banner == 0) {
  print_banner();
}

project_init(\@main_project, "", \@global_settings);

# Fix the file and directory names
fix_file_and_directory_names(".");

# Scan the sources to identify the projects and targets
source_scan();

# Fix the source files
if (! $opt_no_source_fix) {
  fix_source();
}

# Generate the Makefile and the spec file
if (! $opt_no_generated_files) {
  generate();
}
