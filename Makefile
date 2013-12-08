# Copyright 2013 Oliver Katz
#
# This file is part of LibCfg.
#
# LibCfg is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# LibCfg is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with LibCfg.  If not, see <http://www.gnu.org/licenses/>.

CXX=clang++
CXXFLAGS=-std=c++11 -O0 -g -Wall -Wno-write-strings
CXX_INCLUDES=
CXX_LIBS=

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CXX_INCLUDES)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CXX_INCLUDES)

OBJ=Util.o Cfg.o
TEST=TestCfg.o 

build : $(OBJ)

clean :
	rm -rf $(shell echo *.o *.bin */*.o */*.bin) doc/html

test : $(TEST)
	for i in $(TEST); do echo $(CXX) $(CXXFLAGS) $$i $(OBJ) -o $$(dirname $$i)/$$(basename $$i .o).bin $(CXX_INCLUDES) $(CXX_LIBS); $(CXX) $(CXXFLAGS) $$i $(OBJ) -o $$(dirname $$i)/$$(basename $$i .o).bin $(CXX_INCLUDES) $(CXX_LIBS); done

doxygen :
	doxygen doxygen.cfg
	cp customTabs.css doc/html/tabs.css