import pathlib

def get_project_root():
    return str(pathlib.Path(__file__).parent.parent.parent)

