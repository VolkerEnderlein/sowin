# libconfigvars.sh
# no to be executed - this script must be sourced

############################################################################
# Source Substitutions:

WIDGET="HWND"
EVENT="MSG *"
COMPONENTHEADER=""

# new
Gui=Win
gui=win
GUI=WIN

export Gui gui GUI WIDGET EVENT COMPONENTHEADER

############################################################################
# Build Conditionals:

HAVE_RENDERAREA=true
HAVE_EXAMINERVIEWER=false
HAVE_PLANEVIEWER=false
HAVE_WALKVIEWER=false
HAVE_FLYVIEWER=false
HAVE_LIGHTSLIDERSET=false
HAVE_MATERIALSLIDERSET=false
HAVE_TRANSFORMSLIDERSET=false
HAVE_DIRECTIONALLIGHTEDITOR=false
HAVE_MATERIALEDITOR=false
HAVE_MATERIALLIST=false
HAVE_PRINTDIALOG=false

export HAVE_RENDERAREA HAVE_EXAMINERVIEWER
export HAVE_PLANEVIEWER HAVE_WALKVIEWER HAVE_FLYVIEWER
export HAVE_LIGHTSLIDERSET HAVE_MATERIALSLIDERSET HAVE_TRANSFORMSLIDERSET
export HAVE_DIRECTIONALLIGHTEDITOR HAVE_MATERIALEDITOR
export HAVE_MATERIALLIST HAVE_PRINTDIALOG








