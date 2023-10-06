from pybind11_stubgen import ModuleStubsGenerator
import os, sys
import argparse
from pathlib import Path
import importlib

def list_modules(package_path: str):
    package_name = str(package_path).split("/")[-1].split(".")[0]
    package_path = os.path.dirname(importlib.import_module(package_name, package_path).__file__)
    modules = []
    for root, _, files in os.walk(package_path):
        for file in files:
            if file.endswith(".so"):
                file = file.split('.')[0]
                module_name = os.path.splitext(os.path.relpath(os.path.join(root, file), package_path))[0].replace(os.path.sep, ".")
                modules.append((f"{package_name}.{module_name}", root))
    return modules

parser = argparse.ArgumentParser()
parser.add_argument('-p', help='package path')

args = parser.parse_args()
p = Path(args.p).absolute()

sys.path.append(str(p))

modules = list_modules(p)
for module_name, module_path in modules:
    module_name: str
    module = ModuleStubsGenerator(module_name)
    module.parse()
    module_name = module_name.split('.')[-1]
    with open(str(Path(module_path)/f"{module_name}.pyi"), 'w') as f:
        f.write("#\n# Automaticall generated file, do not edit!\n#\n\n")
        f.write("\n".join(module.to_lines()))
