// Open Add Modal
function openAddModal() {
  document.getElementById('addModal').classList.add('show');
}

// Open Edit Modal
function openEditModal(id, name, section, year) {
  document.getElementById('editId').value = id;
  document.getElementById('editIdDisplay').value = id;
  document.getElementById('editName').value = name;
  document.getElementById('editSection').value = section;
  document.getElementById('editYear').value = year;
  document.getElementById('editModal').classList.add('show');
}

// Open Delete Modal
function openDeleteModal(id, name) {
  document.getElementById('deleteId').value = id;
  document.querySelector('.delete-name').textContent = name;
  document.getElementById('deleteModal').classList.add('show');
}

// Close Modal
function closeModal(modalId) {
  document.getElementById(modalId).classList.remove('show');
}

// Close modal when clicking outside
window.onclick = function (event) {
  if (event.target.classList.contains('modal')) {
    event.target.classList.remove('show');
  }
}

// Close modal on ESC key
document.addEventListener('keydown', function (event) {
  if (event.key === 'Escape') {
    const modals = document.querySelectorAll('.modal.show');
    modals.forEach(modal => modal.classList.remove('show'));
  }
});
