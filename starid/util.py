from pathlib import Path


def get_project_root():
    return str(Path(__file__).parent.parent)
