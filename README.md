# 🥷 ShadowSprint

**ShadowSprint** est un runner 2D développé avec SFML. Incarnez un ninja en fuite à travers une ville, esquivez les shurikens, collectez des bonus, et survivez le plus longtemps possible en bloquant, sautant et sprintant à toute vitesse.

---

## 🎮 Gameplay

- **Déplacement automatique** : le joueur avance constamment, la vitesse de la map augmente progressivement avec le score.
- **Saut** : appuyez sur `Espace` pour sauter.
- **Blocage** : appuyez sur `Shift gauche` pour bloquer les projectiles. Le blocage dure le temps de l'animation et déclenche un cooldown de 5s avant de pouvoir de nouveau bloquer.
- **Bonus** :
  - 🛡️ **Invincibilité** : ignore les dégâts pendant 5 secondes.
  - 🐢 **Slow Mode** : ralentit les projectiles ennemis.
  - ✖️ **Score x2** : double temporairement les points gagnés.

---

## 🖥️ Contrôles

| Action         | Touche              |
|----------------|---------------------|
| Saut           | `Espace`            |
| Blocage        | `Shift gauche`      |
| Pause          | `Échap`             |
| Navigation menu| `Souris`            |

## ⚙️ Technologies

- **Langage** : C++17
- **Librairie graphique** : [SFML 3.0+](https://www.sfml-dev.org/)
- **Structure orientée objet** avec animations, entités, menus et bonus modulaires
