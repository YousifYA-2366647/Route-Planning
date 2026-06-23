import pandas as pd
import datashader as ds
import datashader.transfer_functions as tf
from PIL import Image


def plot_california_nodes(file_path, output_path="california_graph_render.png"):
    print("Loading data...")
    df = pd.read_csv(file_path)

    # Check if all columns exist in CSV-file
    required_cols = {'xCoord', 'yCoord', 'visited', 'onPath'}
    if not required_cols.issubset(df.columns):
        raise ValueError(f"CSV must contain columns: {required_cols}")

    # Convert columns to correct types
    df['xCoord'] = df['xCoord'].astype(float)
    df['yCoord'] = df['yCoord'].astype(float)
    df['visited'] = df['visited'].astype(int)
    df['onPath'] = df['onPath'].astype(bool)

    # Check the bounds of the coordinates
    x_min, x_max = df['xCoord'].min(), df['xCoord'].max()
    y_min, y_max = df['yCoord'].min(), df['yCoord'].max()

    print(f"x range: {x_min} → {x_max}")
    print(f"y range: {y_min} → {y_max}")

    cvs = ds.Canvas(
        plot_width=1000,
        plot_height=1000,
        x_range=(x_min, x_max),
        y_range=(y_min, y_max)
    )

    # Split data based on conditions
    onpath_df = df[df['onPath'] == True]
    visited_df = df[(df['visited'] == 1) & (df['onPath'] == False)]
    unvisited_df = df[(df['visited'] == 0) & (df['onPath'] == False)]

    # Render unvisited nodes (grey)
    print("Rendering unvisited nodes...")
    agg_u = cvs.points(unvisited_df, 'xCoord', 'yCoord', agg=ds.count())
    img_u = tf.shade(
        agg_u,
        cmap=["#222222", "#bbbbbb"],
        how="eq_hist"
    )
    base_img = img_u.to_pil().convert("RGBA")

    # Render visited nodes (green)
    if not visited_df.empty:
        print(f"Rendering {len(visited_df)} visited nodes...")
        agg_v = cvs.points(visited_df, 'xCoord', 'yCoord', agg=ds.count())
        img_v = tf.shade(
            agg_v,
            cmap=["#003300", "#00FF00"],
            how="eq_hist"
        )
        pil_v = img_v.to_pil().convert("RGBA")
        base_img = Image.alpha_composite(base_img, pil_v)

    # Render path nodes (red)
    # Render these last so they won't be recoloured by something else
    if not onpath_df.empty:
        print(f"Rendering {len(onpath_df)} path nodes...")
        agg_p = cvs.points(onpath_df, 'xCoord', 'yCoord', agg=ds.count())
        
        # 1. Create the base shaded image
        img_p = tf.shade(
            agg_p,
            cmap=["#FF0000", "#FF0000"],
            how="linear"
        )
        
        # 2. Make pixels thicker for visibility
        img_p = tf.spread(img_p, px=1, shape='circle') 
        
        pil_p = img_p.to_pil().convert("RGBA")
        base_img = Image.alpha_composite(base_img, pil_p)

    # Add black background
    print("Applying background...")
    background = Image.new("RGBA", base_img.size, (0, 0, 0, 255))
    final_img = Image.alpha_composite(background, base_img)

    # Save image to the output path
    print(f"Saving to {output_path}...")
    final_img.save(output_path)

    print("Done")


if __name__ == "__main__":
    plot_california_nodes("nodes.csv")