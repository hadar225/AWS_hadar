import os
import json
import zipfile
import subprocess
from datetime import datetime



project_name = os.getcwd().split("/")[-1]
version = subprocess.check_output(['git','describe','--tags']).decode('utf-8').strip()


def create_gomods_structure():


    # Create .info file
    info_content = {
        "Version":version,
        "Time": datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%SZ")
    }

    with open(f"{version}.info", 'w') as info_file:
        json.dump(info_content, info_file, indent=2)

    # Create zip file
    with zipfile.ZipFile(f"{version}.zip", 'w') as zipf:
        for subdir,_,subfiles in os.walk("."):
            for subfile in subfiles:
                _, file_extention = os.path.splitext(subfile)
                if file_extention  == '.go' or file_extention == '.mod':
                    full_path = os.path.join(subdir, subfile)
                    print(full_path)
                    out_path = os.path.join(f"gitlab.idf.cts/moran/{project_name}@{version}/",full_path)
                    zipf.write(full_path,out_path)
                    
    # Create .mod file
    with open("go.mod", "r") as mod_file:
        mod_file_content = mod_file.read()
    with open(f"{version}.mod","w") as new_mod_file:
        new_mod_file.write(mod_file_content)



if __name__ == "__main__":
    create_gomods_structure()