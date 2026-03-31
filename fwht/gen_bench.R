# Charger les bibliothèques nécessaires
# Installe-les avec install.packages("tidyverse") si ce n'est pas déjà fait
library(tidyverse)
library(scales) 

# 1. Lecture du fichier CSV
# On précise col_names = FALSE car on va renommer proprement pour éviter les problèmes d'espaces
df <- read_csv("benchv2_base_rotatedata_mat.csv", skip = 1, col_names = c("name", "mean_ns", "stddev_pct", "confidence_pct", "drop")) %>%
  select(-drop) # Supprimer la dernière colonne vide causée par la virgule finale

# 2. Extraction des paramètres depuis la colonne "name"
df_parsed <- df %>%
  extract(
    col = name,
    into = c("method", "nrows", "ncols", "ridx1", "ridx2"),
    regex = "^([^.]+)\\.dim_(\\d+)x(\\d+)_rowidx1_(\\d+)_rowidx2_(\\d+)",
    convert = TRUE # Convertit automatiquement les nombres en entiers (integer)
  )

# Affichage de vérification dans la console
print(df_parsed)

# 3. Création du graphique avec ggplot2
plot <- ggplot(df_parsed, aes(x = ncols, y = mean_ns, color = method, group = method)) +
  facet_grid(ridx2 ~ method ) +
  geom_line(linewidth = 1) +
  geom_point(size = 3) +
  
  # Configuration de l'axe X pour afficher les puissances de 2 (2^x)
  scale_x_continuous(
    trans = log2_trans(), 
    breaks = trans_breaks("log2", function(x) 2^x),
    labels = trans_format("log2", math_format(2^.x))
  ) +
  
  # Axe Y : Conversion de nanosecondes en secondes
  scale_y_continuous(
    labels = function(y) y / 1e9  # Divise par 1 milliard
  ) +
  # (Optionnel) Axe Y rendu plus lisible
  #scale_y_continuous(labels = scales::label_number(scale_cut = scales::cut_short_scale())) +
  
  labs(
    title = "Temps d'exécution de la rotation de matrice (NROWS = 2)",
    subtitle = "Comparaison entre rotatedata_mat et rotatedata_mat_rmaj",
    x = expression(Nombre~de~colonnes~(NCOLS~en~2^x)), # expression() permet un rendu mathématique du titre
    y = "Temps moyen (sec)",
    color = "Méthode"
  ) +
  theme_minimal() +
  theme(
    plot.title = element_text(face = "bold"),
    legend.position = "bottom",
    # Plus besoin d'incliner le texte à 45° car "2^10" prend très peu de place en largeur !
    axis.text.x = element_text(size = 11) 
  )

# 4. Affichage et sauvegarde du graphique
print(plot)
ggsave("plot_benchmark_rotatedata.png", plot, width = 8, height = 6, dpi = 300)