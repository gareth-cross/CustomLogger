import argparse
import os
import platform
import shutil
import subprocess
import typing


def create_directory(path: str, clean: bool):
    if clean and os.path.exists(path):
        print(f"Deleting {path}")
        shutil.rmtree(path)
    if not os.path.exists(path):
        print(f"Creating {path}")
        os.makedirs(path)
    return path


def invoke(command_args: typing.List[str], *args, **kwargs):
    cmd_str = " ".join(command_args)
    print(f"Running: {cmd_str}")
    subprocess.check_call(command_args, *args, **kwargs)


def main(args: argparse.Namespace):
    # Get the directory of the project
    current_path = os.path.realpath(__file__)
    project_path = os.path.dirname(current_path)
    third_party_src_path = os.path.join(project_path, "ThirdParty")

    # make a build directory
    build_path = create_directory(
        path=os.path.join(third_party_src_path, "build", args.platform),
        clean=args.clean,
    )

    # run cmake
    cmake_cmd = ["cmake", third_party_src_path]
    invoke(cmake_cmd, cwd=build_path)

    # run cmake to build and install
    cmake_build_cmd = ["cmake", "--build", build_path, "--config", "RelWithDebInfo", "--target", "Install"]
    invoke(cmake_build_cmd, cwd=build_path)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--clean", action="store_true", help="Clean the third party directory"
    )
    parser.add_argument("--platform", type=str, default=platform.system(), help="Platform to build for")
    return parser.parse_args()


if __name__ == "__main__":
    main(parse_args())
