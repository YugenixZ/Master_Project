#!/bin/bash

# Create gap settings files for requested instances
# Format: instance_name gap_value

declare -A instances=(
    ["swath2"]="0.1356"
    ["eli33-2"]="0.1263"
    ["network09"]="1.6857"
    ["network10"]="3.265"
    ["network12"]="5.8266"
    ["network13"]="7.343"
)

# Template directories
TEMPLATE_FULLSTRONG="/scratch/htc/yzhou/exp_scipmip/settings/sandbox_fullstrong.set"
TEMPLATE_GENDJ="/scratch/htc/yzhou/exp_scipmip/settings/sandbox_gendj.set"
TEMPLATE_RELPSCOST="/scratch/htc/yzhou/exp_scipmip/settings/sandbox_relpscost.set"

# Generate files for each instance and each branching rule
for instance in "${!instances[@]}"; do
    gap_value="${instances[$instance]}"
    
    echo "Creating gap settings for $instance with gap $gap_value"
    
    # Fullstrong
    output_file="sandbox_fullstrong_gap${gap_value}_${instance}.set"
    cp "$TEMPLATE_FULLSTRONG" "$output_file"
    echo "" >> "$output_file"
    echo "# Gap settings for instance: $instance" >> "$output_file"
    echo "# Generated from: sandbox_fullstrong.set" >> "$output_file"
    echo "limits/gap = $gap_value" >> "$output_file"
    echo "limits/absgap = 1e-6" >> "$output_file"
    
    # GenDJ
    output_file="sandbox_gendj_gap${gap_value}_${instance}.set"
    cp "$TEMPLATE_GENDJ" "$output_file"
    echo "" >> "$output_file"
    echo "# Gap settings for instance: $instance" >> "$output_file"
    echo "# Generated from: sandbox_gendj.set" >> "$output_file"
    echo "limits/gap = $gap_value" >> "$output_file"
    echo "limits/absgap = 1e-6" >> "$output_file"
    
    # RelPSCost
    output_file="sandbox_relpscost_gap${gap_value}_${instance}.set"
    cp "$TEMPLATE_RELPSCOST" "$output_file"
    echo "" >> "$output_file"
    echo "# Gap settings for instance: $instance" >> "$output_file"
    echo "# Generated from: sandbox_relpscost.set" >> "$output_file"
    echo "limits/gap = $gap_value" >> "$output_file"
    echo "limits/absgap = 1e-6" >> "$output_file"
done

echo "Gap settings generation complete!"