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

def run_executable_with_file_input(executable_path, input_file_path, omp_threads=None):
    env = os.environ.copy()
    if omp_threads is not None:
        env["OMP_NUM_THREADS"] = str(omp_threads)

    with open(input_file_path, 'r') as infile:
        result = subprocess.run(
            [executable_path],
            stdin=infile,
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

    results = []
    input_files = sorted(os.listdir(input_dir))

    # Sequential
    print("⚙️ Running sequential implementation...")
    for filename in input_files:
        input_path = os.path.join(input_dir, filename)
        reading, processing, total = run_executable_with_file_input(sequential_executable, input_path)
        results.append([
            os.path.basename(sequential_executable),
            filename,
            '',  # No threads for sequential
            reading,
            processing,
            total
        ])

    # Parallel
    print("⚙️ Running parallel implementation (OMP_NUM_THREADS 1–20)...")
    for filename in input_files:
        input_path = os.path.join(input_dir, filename)
        for threads in range(1, 21):
            reading, processing, total = run_executable_with_file_input(parallel_executable, input_path, omp_threads=threads)
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
