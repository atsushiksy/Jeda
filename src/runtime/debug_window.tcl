#!/usr/local/bin/wish
#   Copyright (C) 2002 Juniper Networks Inc.
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   f_ree Software f_oundation; either version 2, or (at your option) any
#   later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or f_ITNESS f_OR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the f_ree Software
#   f_oundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
#   USA.  
#

# f_irst argument is the socket port number
set SocketPort [lindex $argv 0]
set MergePid [lindex $argv 1]
# initialize global variables
set Line ""
set ReadState "Idle"
set WhichLine 0
set Filename ""
set Format ""

# Color of the windows 
set Cscrollb "SkyBlue"
set Cbuttonf "black"
set Cbuttonb "SkyBlue"
set Cquitbuttonb "red"
set Clabelf "black"
set Clabelb "SkyBlue"
set Centryf "black"
set Centryb "white"
set Ctextf "black"
set Ctextb "white"

##### Window structure construction ####
#buttons on f_info: <print exp> <where> <show vars> <show lvars> <up> <down> 
#entry on f_command : Command [ <field> ]
#buttons on f_sim_cntl : <break sel> <tbreak sel> <clear break sel> 
#                           <continue> <step> <step any> <next> <quit>
#f_brklst shows the list of breakpoint
#f_thread shows the list of threads
#f_file shows the file name
#f_source shows the current source
frame .f_info
frame .f_command
frame .f_sim_cntl
frame .f_brklst
frame .f_thread
frame .f_file
frame .f_source

#buttons on f_info: <print exp> <show vars> <show lvars> <where> <up> <down> 
button .f_info.bt0 -text "print <expression>" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_print_command }

label .f_info.flabel -text "Fmt:" \
  -foreground $Clabelf -background $Clabelb 

menubutton .f_info.format -text "as is" \
  -menu .f_info.format.menu -relief sunken -width 4
menu .f_info.format.menu  
.f_info.format.menu add command -label "as is" \
  -command set_format_asis
.f_info.format.menu add command -label "hex" \
  -command set_format_hex
.f_info.format.menu add command -label "oct" \
  -command set_format_oct
.f_info.format.menu add command -label "dec" \
  -command set_format_dex
.f_info.format.menu add command -label "bin" \
  -command set_format_bin

button .f_info.bt1 -text "show vars" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_show_vars_command }
  
button .f_info.bt2 -text "show lvars" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_show_lvars_command }
  
button .f_info.bt3 -text "where" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_where_command }
  
button .f_info.bt4 -text "up" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_up_command }
  
button .f_info.bt5 -text "down" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_down_command }

#entry on f_command : Command [ <field> ]
label .f_command.label -text "Command:" \
  -foreground $Clabelf -background $Clabelb 
  
entry .f_command.entry -textvariable Command -width 78 \
  -foreground $Centryf -background $Centryb
  
#buttons on f_sim_cntl : <continue> <step> <step any> <next> <quit>
#buttons on f_break : <break sel> <tbreak sel> <clear break sel>
button .f_sim_cntl.bt0 -text "break <sel>" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_break_command }

button .f_sim_cntl.bt1 -text "tbreak <sel>" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_tbreak_command }

button .f_sim_cntl.bt2 -text "clear <sel>" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_clear_command }

button .f_sim_cntl.bt3 -text "continue" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_continue_command }
  
button .f_sim_cntl.bt4 -text "step" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_step_command }

button .f_sim_cntl.bt5 -text "step any" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_stepany_command }

button .f_sim_cntl.bt6 -text "next" \
  -foreground $Cbuttonf -background $Cbuttonb \
  -command { exec_next_command }

button .f_sim_cntl.bt7 -text "quit" \
  -foreground $Cbuttonf -background $Cquitbuttonb \
  -command { exec_quit_command }

#f_brklst shows the list of breakpoint
label .f_brklst.label -text "breakpoints" \
  -foreground $Clabelf -background $Clabelb 

listbox .f_brklst.blist -height 5 \
  -foreground $Ctextf -background $Ctextb \
 -xscrollcommand ".f_brklst.scrollx set" \
 -yscrollcommand ".f_brklst.scrolly set" 

scrollbar .f_brklst.scrollx -orient horizontal \
  -background $Cscrollb \
  -command ".f_brklst.blist xview" 
  
scrollbar .f_brklst.scrolly \
  -background $Cscrollb \
  -command ".f_brklst.blist yview"


#f_thread shows the list of threads
label .f_thread.label -text "threads" \
  -foreground $Clabelf -background $Clabelb 

listbox .f_thread.thrdlist -height 5 \
  -foreground $Ctextf -background $Ctextb \
 -xscrollcommand ".f_thread.scrollx set" \
 -yscrollcommand ".f_thread.scrolly set" 


scrollbar .f_thread.scrollx -orient horizontal \
  -background $Cscrollb \
  -command ".f_thread.thrdlist xview" 
  
scrollbar .f_thread.scrolly \
  -background $Cscrollb \
  -command ".f_thread.thrdlist yview"

#entry on f_file : f_ile [ <field> ]
label .f_file.label -text "File:" \
  -foreground $Clabelf -background $Clabelb 
  
entry .f_file.entry -textvariable Filename -width 80 \
  -foreground $Centryf -background $Centryb

#f_source shows the current source
text .f_source.text -relief sunken -bd 2 \
  -foreground $Ctextf -background $Ctextb \
  -xscrollcommand ".f_source.scrollx set"  \
  -yscrollcommand ".f_source.scrolly set" \
  -state disabled -wrap none

scrollbar .f_source.scrollx -orient horizontal \
  -background $Cscrollb \
  -command ".f_source.text xview" 

scrollbar .f_source.scrolly \
  -background $Cscrollb  \
  -command ".f_source.text yview" 


##### packing frames

## f_info frame
pack .f_info.bt0 .f_info.flabel .f_info.format .f_info.bt1 .f_info.bt2 .f_info.bt3 .f_info.bt4 .f_info.bt5 \
  -side left

## f_command frame
pack .f_command.label .f_command.entry -side left

## f_sim_cntl frame
pack .f_sim_cntl.bt0 .f_sim_cntl.bt1 .f_sim_cntl.bt2 .f_sim_cntl.bt3 \
  .f_sim_cntl.bt4 .f_sim_cntl.bt5 .f_sim_cntl.bt6 .f_sim_cntl.bt7 \
  -side left

## f_brklst frame
pack .f_brklst.label -side top -fill x 
pack .f_brklst.scrolly -side right -fill y
pack .f_brklst.scrollx -side bottom -fill x
pack .f_brklst.blist -expand 1 -fill both 

## f_thread frame
pack .f_thread.label -side top -fill x
pack .f_thread.scrolly -side right -fill y
pack .f_thread.scrollx -side bottom -fill x
pack .f_thread.thrdlist -expand 1 -fill both 

## f_file frame
pack .f_file.label .f_file.entry -side left

##f_source frame
pack .f_source.scrolly -side right -fill y
pack .f_source.scrollx -side bottom -fill x
pack .f_source.text -side bottom -expand 1 -fill both 

##Top frame
pack .f_info
pack .f_command
pack .f_sim_cntl
pack .f_brklst  -expand 1 -fill both
pack .f_thread  -expand 1 -fill both
pack .f_file 
pack .f_source  -expand 1 -fill both


# Command field action
bind .f_command.entry <Key-Return> {
  puts $Command
}

# Break list field double click action
bind .f_brklst.blist <Double-Button-1> {
  set brk [selection get]
  exec_show_break $brk
}


# Thread field double click action
bind .f_thread.thrdlist <Double-Button-1> {
  set thrd [selection get]
  exec_show_thread $thrd
}

# File field action
bind .f_file.entry <Key-Return> {
  exec_load_file $Filename
}

if { $SocketPort != "" } {
  #Open socket for communication
  #puts stderr "Socport = $SocketPort"
  set jdbSocket [socket localhost $SocketPort]
  fconfigure $jdbSocket -buffering line
  fileevent $jdbSocket readable exec_read_loop
}

set Format ""
puts "update $MergePid"





## Source text display procedures

proc exec_show_thread thrdline {
  if { [regexp {ThreadID\(([0-9]+)\)} $thrdline x id] } {
    puts "thread $id"
  }
}

proc exec_load_file fname {
  global WhichLine
  global Filename
  #puts stderr "exec_load_file loading $fname\n"
  .f_source.text configure -state normal
  .f_source.text delete 1.0 end
  set f [open $fname]
  set ln 0
  while {![eof $f]} {
    gets $f line
    set ln [expr $ln + 1]
    set lns [format "  %5d " $ln]
    .f_source.text insert end $lns
    .f_source.text insert end $line
    .f_source.text insert end "\n"
  }
  close $f
  .f_source.text configure -state disabled
  set Filename $fname
  set WhichLine 0
}

proc show_current lnum {
  global WhichLine
  #puts stderr "show_current $lnum $WhichLine\n"
  .f_source.text configure -state normal
  if { $WhichLine > 0 } {
    .f_source.text delete $WhichLine.1 $WhichLine.2
    .f_source.text insert $WhichLine.1 " "
    set WhichLine 0
  }
  .f_source.text delete $lnum.1 $lnum.2
  .f_source.text insert $lnum.1 ">"
  .f_source.text configure -state disabled
  .f_source.text yview [expr $lnum - 5]
  set WhichLine $lnum
}

proc show_break lnum {
  .f_source.text configure -state normal
  .f_source.text delete $lnum.0 $lnum.1
  .f_source.text insert $lnum.0 "B"
  .f_source.text configure -state disabled
}

proc show_tbreak lnum {
  .f_source.text configure -state normal
  .f_source.text delete $lnum.0 $lnum.1
  .f_source.text insert $lnum.0 "T"
  .f_source.text configure -state disabled
}

proc show_clear_break lnum {
  .f_source.text configure -state normal
  .f_source.text delete $lnum.0 $lnum.1
  .f_source.text insert $lnum.0 " "
  .f_source.text configure -state disabled
}

## exec button function procedures
proc exec_print_command { } {
  global Format
  set expression [selection get]
  if { $expression != "" } {
    puts "print $Format $expression" 
  }
}

proc exec_show_vars_command { } {
  puts "show vars"
}

proc exec_show_lvars_command { } {
  puts "show lvars"
}

proc exec_where_command { } {
  puts "where"
}
  
proc exec_up_command { } {
  puts "up"
}
  
proc exec_down_command { } {
  puts "down"
}


proc exec_break_command { } {
  global Filename
  if { [selection own] == ".f_source.text" } {
    set l [ .f_source.text index sel.first ]
    set xy [split $l \.]
    set y [lindex $xy 0]
    puts "break $Filename:$y"
  }
}

proc exec_tbreak_command { } {
  global Filename
  if { [selection own] == ".f_source.text" } {
    set l [ .f_source.text index sel.first ]
    set xy [split $l \.]
    set y [lindex $xy 0]
    puts "tbreak $Filename:$y"
  }
}

proc exec_clear_command { } {
  global Filename
  if { [selection own] == ".f_source.text" } {
    set l [ .f_source.text index sel.first ]
    set xy [split $l \.]
    set y [lindex $xy 0]
    puts "clear $Filename:$y"
  } elseif { [selection own] == ".f_brklst.blist" } {
    set brk [selection get]
    puts "clear $brk"  
  }
}

proc exec_continue_command { } {
  puts "continue"
}
proc exec_step_command { } {
  puts "step"
}
proc exec_stepany_command { } {
  puts "stepany"
}
proc exec_next_command { } {
  puts "next"
}
proc exec_quit_command { } {
  puts "quit"
}


## format menu selecton procedures
proc set_format_asis { } {
  global Format
  set Format ""
  .f_info.format configure -text "as is"
}

proc set_format_hex { } {
  global Format
  set Format "/x"
  .f_info.format configure -text "hex"
}

proc set_format_oct { } {
  global Format
  set Format "/o"
  .f_info.format configure -text "oct"
}

proc set_format_dex { } {
  global Format
  set Format "/d"
  .f_info.format configure -text "dec"
}

proc set_format_bin { } {
  global Format
  set Format "/b"
  .f_info.format configure -text "bin"
}

proc exec_read_loop { } {
  global jdbSocket
  global ReadState
  global Filename
  global Line
  
  gets $jdbSocket Line
   
  if { $Line=="" } {
  } elseif { $ReadState=="Idle" } {
    if { $Line=="#file" } {
      set ReadState "File"
    } elseif { $Line=="#location" } {
      set ReadState "Location"
    } elseif { $Line=="#breaklist" } {
      set ReadState "Breaklist"
      .f_brklst.blist delete 0 end
    } elseif { $Line=="#tbreakpoint" } {
      set ReadState "Tbreakpoint"
    } elseif { $Line=="#breakpoint" } {
      set ReadState "Breakpoint"
    } elseif { $Line=="#clearpoints" } {
      set ReadState "Clearpoints"
    } elseif { $Line=="#threadlist" } {
      set ReadState "Threadlist"
      .f_thread.thrdlist delete 0 end
    } else {
      puts stderr "$Line?" ;
    }    
  } elseif { $ReadState=="File" } {
    exec_load_file $Line
    set ReadState "Idle"
  } elseif { $ReadState=="Location" } {
    show_current $Line
    set ReadState "Idle"
  } elseif { $ReadState=="Breaklist" } {
    if { $Line=="#end" } {
      set ReadState "Idle"
    } elseif { $Line!="" } {
      .f_brklst.blist insert end $Line
    }
  } elseif { $ReadState=="Threadlist" } {
    if { $Line=="#end" } {
      set ReadState "Idle"
    } elseif { $Line!="" } {
      .f_thread.thrdlist insert end $Line
    }
  } elseif { $ReadState == "Breakpoint" } {
    if { $Line!="" } {
      show_break $Line
      set ReadState "Idle"
    }
  } elseif { $ReadState=="Tbreakpoint" } {
    if { $Line!="" } {
      show_tbreak $Line
      set ReadState "Idle"
    }
  } elseif { $ReadState=="Clearpoints" } {
    if { $Line!="" } {
      show_clear_break $Line
      set ReadState "Idle"
    }
  }
  
}
