cmake_minimum_required(VERSION 2.6)
project(pong)

find_package(Gettext)

file(GLOB pofiles *.po)

set(_potFile pong--11.pot)
set(XGETTEXT_CMD xgettext)
set(_xgettext_option_list -k_ --force-po --package-name=pong--11)
set(_src_list ${CMAKE_SOURCE_DIR}/src/game.cpp
              ${CMAKE_SOURCE_DIR}/src/main.cpp)

GETTEXT_CREATE_TRANSLATIONS(${_potFile} ALL ${pofiles})

# http://stackoverflow.com/questions/2694742/is-there-a-cmake-function-to-update-pot-files#11606896
add_custom_command(OUTPUT ${_potFile}
  COMMAND ${XGETTEXT_CMD} ${_xgettext_option_list} -o ${_potFile} ${_src_list}
  DEPENDS ${_src_list}
  COMMENT "Extract translatable messages to ${_potFile}"
)

add_custom_target(pot_file DEPENDS ${_potFile})
