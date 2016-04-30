#!/usr/bin/make -f
MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE))))
BUILDDIR ?= build

.PHONY:		default
default:	all

PACKAGE := sc-memory
DESCRIPTION := A memory controller and DRAM component in SystemC
VERSION := 0.2

include gnudirs.mk

SHELL = /bin/sh
CPP ?= cpp
CPPFLAGS ?=
CC ?= gcc
CXX ?= g++
AR ?= ar
CFLAGS ?= -g -O2 -fPIC
CXXFLAGS ?= -g -O2 -std=c++11 -fPIC
LDFLAGS ?=
LDLIBS ?=
ARFLAGS ?= rcs

REQUIREMENTS := systemc expio
OPTIONALS :=

include rules.mk
