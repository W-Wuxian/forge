library(tidyverse)
library(scales)
library(plotly)

args <- commandArgs(trailingOnly = TRUE)
csv_name <- tools::file_path_sans_ext(basename(args[1]))

# 1. Lecture du fichier CSV
df <- read_csv(args[1], skip = 1, col_names = c("name", "mean_ns", "stddev_pct", "confidence_pct", "drop")) %>%
  select(-drop)

# 2. Extraction des paramètres depuis la colonne "name"
df_parsed <- df %>%
  extract(
    col = name,
    into = c("method", "nrows", "ncols", "sketch_dim", "sketch_alg", "sketch_type", "omp_thread"),
    regex = "^([^.]+)\\.dim_(\\d+)x(\\d+)_sketch_dim_(\\d+)_sketch_alg_(\\w+)_sketch_type_(\\w+)_openmp_(\\d+)",
    convert = TRUE
  ) %>%
  mutate(
    mean_s        = mean_ns / 1e9,
    sd_s          = mean_s * (stddev_pct / 100),
    omp_thread    = factor(omp_thread, levels = sort(unique(omp_thread))),
    nrows_lab     = paste0("nrows = ", nrows),
    sketch_dim_lab = paste0("sketch_dim = ", sketch_dim),
    thread_lab    = factor(paste0(omp_thread, " threads"),
                            levels = paste0(sort(unique(omp_thread)), " threads"))
  )

stopifnot(all(c("nrows_lab", "sketch_dim_lab", "thread_lab", "sketch_alg", "sketch_type", "mean_s") %in% colnames(df_parsed)))
print(df_parsed)

# 3. Création du graphique avec ggplot2
sketch_plot <- ggplot(
  df_parsed,
  aes(x = factor(ncols), y = mean_s, color = sketch_alg,
      linetype = sketch_type, group = interaction(sketch_alg, sketch_type))
) +
  facet_grid(rows = vars(nrows_lab, sketch_dim_lab), cols = vars(thread_lab),
             labeller = label_value, scales = "free_x") +
  geom_line(linewidth = 0.9) +
  geom_point(size = 2.2) +
  geom_errorbar(
    aes(ymin = mean_s - sd_s, ymax = mean_s + sd_s),
    width = 0.15, alpha = 0.5
  ) +
  scale_color_viridis_d(option = "D", end = 0.85) +
  scale_y_continuous(
    labels = label_number(accuracy = 0.001),
    expand = expansion(mult = c(0.05, 0.1))
  ) +
  labs(
    title    = "Sketch block (no comm) — temps d'exécution",
    subtitle = "Temps moyen par taille de matrice, dimension de sketch et nombre de threads OpenMP",
    x        = "Nombre de colonnes",
    y        = "Temps moyen (sec)",
    color    = "Algorithme de sketch",
    linetype = "Type de sketch",
    caption  = "Facettes : lignes = nrows × sketch_dim, colonnes = threads OpenMP"
  ) +
  guides(
    color    = guide_legend(nrow = 2, override.aes = list(linewidth = 1.2)),
    linetype = guide_legend(nrow = 1)
  ) +
  theme_minimal(base_size = 12) +
  theme(
    plot.title       = element_text(face = "bold", size = 14),
    plot.subtitle    = element_text(size = 10, color = "grey30"),
    plot.caption     = element_text(size = 8, color = "grey50", hjust = 0),
    legend.position  = "bottom",
    legend.box       = "vertical",
    strip.background = element_rect(fill = "grey90", color = NA),
    strip.text       = element_text(face = "bold", size = 8),
    axis.text.x      = element_text(size = 8, angle = 45, hjust = 1),
    panel.spacing    = unit(0.5, "lines"),
    panel.grid.minor = element_blank()
  )

# 4. Affichage et sauvegarde du graphique
ggsave(paste0(csv_name, ".pdf"), sketch_plot, device = "pdf", width = 12, height = 9, dpi = 300)
ggsave(paste0(csv_name, ".png"), sketch_plot, width = 12, height = 9, dpi = 300)

# Conversion en plot interactif
interactive_plot <- ggplotly(sketch_plot) %>%
  config(scrollZoom = TRUE)

# Sauvegarde en HTML
htmlwidgets::saveWidget(interactive_plot, paste0(csv_name, ".html"))