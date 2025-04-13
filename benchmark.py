import os
import subprocess
import csv

# === Configuration ===
input_dir = 'inputs'
generator_executable = './gen'  # change if different
sequential_executable = './seq'
parallel_executable = './para'
output_csv = 'benchmark_results.csv'
node_counts = [100, 1000, 10000, 100000, 1000000, 10000000]
# ======================

def generate_inputs():
    os.makedirs(input_dir, exist_ok=True)

    for count in node_counts:
        input_path = os.path.join(input_dir, f'input{count}')
        with open(input_path, 'w') as outfile:
            subprocess.run([generator_executable, str(count)], stdout=outfile)
        print(f"✅ Generated: {input_path}")

def scan_directory_to_arrays(directory_path):
    file_data = {}

    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        if os.path.isfile(file_path):
            with open(file_path, 'r', encoding='utf-8') as file:
                lines = file.readlines()
                file_data[filename] = [line.strip() for line in lines]

    return file_data

def run_executable_with_input(executable_path, input_data, omp_threads=None):
    input_str = '\n'.join(input_data) + '\n'
    env = os.environ.copy()
    env["OMP_PROC_BIND"] = 'close'
    env["OMP_PLACES"] = 'cores'
    if omp_threads is not None:
        env["OMP_NUM_THREADS"] = str(omp_threads)

    result = subprocess.run(
        [executable_path],
        input=input_str,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        env=env
    )

    output_lines = result.stdout.strip().split('\n')

    if len(output_lines) < 3:
        return None, None, None

    last_three = output_lines[-3:]

    def extract_time(line):
        try:
            return float(line.strip().split()[-2])
        except:
            return None

    reading_time = extract_time(last_three[0])
    processing_time = extract_time(last_three[1])
    total_time = extract_time(last_three[2])

    return reading_time, processing_time, total_time

def write_csv(results, output_path):
    with open(output_path, mode='w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Executable", "Input File", "OMP_NUM_THREADS", "Reading Time", "Processing Time", "Total Time"])
        for row in results:
            writer.writerow(row)

def main():
    print("🚀 Generating input files...")
    generate_inputs()

    print("📂 Reading inputs...")
    file_arrays = scan_directory_to_arrays(input_dir)
    results = []

    # Sequential
    print("⚙️ Running sequential implementation...")
    for filename, contents in file_arrays.items():
        reading, processing, total = run_executable_with_input(sequential_executable, contents)
        results.append([
            os.path.basename(sequential_executable),
            filename,
            '',  # OMP_NUM_THREADS not applicable
            reading,
            processing,
            total
        ])

    # Parallel
    print("⚙️ Running parallel implementation (OMP_NUM_THREADS 1–20)...")
    for filename, contents in file_arrays.items():
        for threads in range(1, 21):
            reading, processing, total = run_executable_with_input(parallel_executable, contents, omp_threads=threads)
            results.append([
                os.path.basename(parallel_executable),
                filename,
                threads,
                reading,
                processing,
                total
            ])

    print("💾 Writing results to CSV...")
    write_csv(results, output_csv)
    print(f"\n✅ Benchmark complete! Results saved to: {output_csv}")

if __name__ == '__main__':
    main()
