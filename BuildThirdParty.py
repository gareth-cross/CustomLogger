import argparse
import os
import platform
import shutil
import subprocess
import typing

CLANG_ROOT = os.path.expanduser(os.path.join('~', 'v19_clang-11.0.1-centos7'))
CLANG_SYSROOT = os.path.join(CLANG_ROOT, 'x86_64-unknown-linux-gnu')


# horrible hardcoded path for use in WSL
# UE_PATH = '/mnt/d/UE_427/UnrealEngine/Engine/Source/ThirdParty/Linux/LibCxx/lib/Linux/x86_64-unknown-linux-gnu'


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
    output_dir = platform.system() if not args.linux else "Linux"
    build_path = create_directory(
        path=os.path.join(third_party_src_path, "build", output_dir),
        clean=args.clean,
    )

    cmake_cmd = ["cmake", third_party_src_path]
    env = os.environ.copy()

    # update the CC and CXX paths to point to clang
    install_target = 'Install'
    if platform.system() == "Linux":
        if not os.path.exists(CLANG_ROOT):
            raise RuntimeError(f'Expected to find Unreal Clang Toolchain at: {CLANG_ROOT}')
        env.update(CC=os.path.join(CLANG_SYSROOT, 'bin', 'clang'))      
        env.update(CXX=os.path.join(CLANG_SYSROOT, 'bin', 'clang++'))
        install_target = 'install'
    elif platform.system() == "Windows" and args.linux:
        # TODO(gareth): This doesn't work yet, not sure why.
        toolchain_path = os.path.join(third_party_src_path, "UnrealLinuxToolchain.cmake")
        cmake_cmd.extend([
            f'-DCMAKE_TOOLCHAIN_FILE={toolchain_path}',
        ])

    # run w/ custom env
    invoke(cmake_cmd, cwd=build_path, env=env)

    # run cmake to build and install
    cmake_build_cmd = ["cmake", "--build", build_path, "--config", "RelWithDebInfo", "--target", install_target]
    if platform.system() == "Linux":
        cmake_build_cmd.extend(['--parallel', str(8)])
    invoke(cmake_build_cmd, cwd=build_path)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--clean", action="store_true", help="Clean the third party directory"
    )
    parser.add_argument("--linux", action="store_true", help="Build linux on windows.")
    return parser.parse_args()


if __name__ == "__main__":
    main(parse_args())
