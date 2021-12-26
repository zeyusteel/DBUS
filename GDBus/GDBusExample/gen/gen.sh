#!/bin/bash
gdbus-codegen --interface-prefix org.gtk.GDBus.Example.ObjectManager. \
              --generate-c-code gen                       \
              --c-namespace Example                       \
              --c-generate-object-manager                 \
              --generate-docbook generated-docs                       \
              gen.xml
